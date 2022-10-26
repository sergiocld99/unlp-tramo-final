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

def resolverEj4(clientesDF, cajasDF):
  windowPorPais = Window.partitionBy('nacionalidad').orderBy('saldo')
  windowPorGrupo = Window.partitionBy('nacionalidad', 'grupo')

  clientesConCajasDF = clientesDF.join(cajasDF, clientesDF.id_cliente == cajasDF.id_cliente, 'inner')

  consulta5 = clientesConCajasDF.withColumn('grupo', F.ntile(3).over(windowPorPais))
  consulta5 = consulta5.withColumn('min', F.min('saldo').over(windowPorGrupo))
  consulta5 = consulta5.withColumn('max', F.max('saldo').over(windowPorGrupo))
  consulta5 = consulta5.withColumn('avg', F.avg('saldo').over(windowPorGrupo))
  consulta5 = consulta5.select('nacionalidad', 'grupo', 'min', 'max', 'avg').dropDuplicates()
  consulta5.show()


# ------------------ PROGRAMA PRINCIPAL --------------------------

clientesDF = cargarTablaClientes()
cajasDF = cargarTablaCajas()

resolverEj4(clientesDF, cajasDF)