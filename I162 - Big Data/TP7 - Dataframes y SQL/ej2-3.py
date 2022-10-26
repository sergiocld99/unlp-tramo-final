root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import sys
sys.path.append('/content/' + root_path)

inputDir = root_path + "Banco/"

# --------------------- SQL CONTEXT -------------------

from pyspark.sql import SQLContext, Row, Window
from pyspark.sql import functions as F

sqlContext = SQLContext(sc)


# ------------------- formato de datasets ----------------------

# Cliente: <ID_Cliente, nombre, apellido, DNI, fecha de nacimiento, nacionalidad>
# CajaDeAhorro: <ID_Caja, ID_Cliente, saldo>
# Prestamos: <ID_Caja, cuotas, monto>
# Movimientos: <ID_Caja, monto, timestamp>


# ------------------------------------------------------------

def cargarTablaClientes():
  clientes = sc.textFile(inputDir + "clientes")       # cargamos el directorio
  clientes = clientes.map(lambda t: t.split('\t'))    # separamos en columnas
  clientes = clientes.map(lambda t: Row(id_cliente = t[0], anio_nac = t[4].split('-')[0], nacionalidad = t[5] ))
  clientesDF = sqlContext.createDataFrame(clientes)   # crear dataframe usando rows
  clientesDF.registerTempTable("Cliente")             # creamos la tabla sql de clientes
  return clientesDF

def cargarTablaCajas():
  cajas = sc.textFile(inputDir + "cajas")
  cajas = cajas.map(lambda t: t.split('\t'))
  cajas = cajas.map(lambda t: Row(id_caja = t[0], id_cliente = t[1], saldo = float(t[2]) ) )
  cajasDF = sqlContext.createDataFrame(cajas)
  cajasDF.registerTempTable("Caja")
  return cajasDF

def cargarTablaPrestamos():
  prestamos = sc.textFile(inputDir + "prestamos")
  prestamos = prestamos.map(lambda t: t.split('\t'))
  prestamos = prestamos.map(lambda t: Row(id_caja = t[0], monto = t[2]) )
  prestamosDF = sqlContext.createDataFrame(prestamos)
  prestamosDF.registerTempTable("Prestamo")
  return prestamosDF

def revisionSQL():
  top3 = sqlContext.sql("SELECT anio_nac, COUNT(*) as cant_clientes FROM Cliente GROUP BY anio_nac SORT BY 2 desc LIMIT 3")
  print(top3.collect())

  top3 = sqlContext.sql("SELECT anio_nac, COUNT(*) as cant_cajas " + 
                      "FROM Cliente Cl INNER JOIN Caja CA ON Cl.id_cliente = CA.id_cliente " + 
                      "GROUP BY anio_nac SORT BY 2 desc LIMIT 3")
  print(top3.collect())

  top3 = sqlContext.sql("SELECT anio_nac, SUM(P.monto) as dinero_prestado " + 
                      "FROM Cliente Cl INNER JOIN Caja CA ON Cl.id_cliente = CA.id_cliente INNER JOIN Prestamo P ON CA.id_caja = P.id_caja " + 
                      "GROUP BY anio_nac SORT BY 2 desc LIMIT 3")
  print(top3.collect())

  top3 = sqlContext.sql("SELECT anio_nac, SUM(CA.saldo) as deuda " + 
                      "FROM Cliente Cl INNER JOIN Caja CA ON Cl.id_cliente = CA.id_cliente WHERE CA.saldo < 0 " + 
                      "GROUP BY anio_nac SORT BY 2 LIMIT 3")

  print(top3.collect())

def resolver(clientesDF, cajasDF, prestamosDF, columna):
  # Crear ventana
  ventana = Window.partitionBy(columna)

  # Consulta 1: TOP 3 DE CANTIDAD DE CLIENTES
  consulta1 = clientesDF.withColumn("cant_clientes", F.count("id_cliente").over(ventana))
  consulta1 = consulta1.select(columna, "cant_clientes").dropDuplicates()
  consulta1 = consulta1.sort(consulta1.cant_clientes.desc())
  consulta1.limit(3).show()

  # Consulta 2: TOP 3 DE CANTIDAD DE CAJAS DE AHORRO EN TOTAL
  clientesConCajasDF = clientesDF.join(cajasDF, clientesDF.id_cliente == cajasDF.id_cliente, 'inner')
  consulta2 = clientesConCajasDF.withColumn('cant_cajas', F.count('id_caja').over(ventana))
  consulta2 = consulta2.select(columna, 'cant_cajas').dropDuplicates()
  consulta2 = consulta2.sort(consulta2.cant_cajas.desc())
  consulta2.limit(3).show()

  # Consulta 3: TOP 3 DE CANTIDAD DE DINERO PRESTADO EN PRESTAMOS
  clientesPrestamosDF = clientesConCajasDF.join(prestamosDF, clientesConCajasDF.id_caja == prestamosDF.id_caja, 'inner')
  consulta3 = clientesPrestamosDF.withColumn('dinero_prestado', F.sum('monto').over(ventana))
  consulta3 = consulta3.select(columna, 'dinero_prestado').dropDuplicates()
  consulta3 = consulta3.sort(consulta3.dinero_prestado.desc())
  consulta3.limit(3).show()

  # Consulta 4: TOP 3 DE CLIENTES MAS DEUDORES (ACUMULADO EN CAJAS CON SALDO NEGATIVO)
  clientesDeudoresDF = clientesConCajasDF.filter("saldo < 0")
  consulta4 = clientesDeudoresDF.withColumn('deuda', F.sum(- clientesDeudoresDF.saldo).over(ventana))
  consulta4 = consulta4.select(columna, 'deuda').dropDuplicates()
  consulta4 = consulta4.sort(consulta4.deuda.desc())
  consulta4.limit(3).show()


# ------------------ PROGRAMA PRINCIPAL --------------------------

clientesDF = cargarTablaClientes();
cajasDF = cargarTablaCajas();
prestamosDF = cargarTablaPrestamos();

resolver(clientesDF, cajasDF, prestamosDF, columna = 'nacionalidad')

# Revisión general
# revisionSQL()