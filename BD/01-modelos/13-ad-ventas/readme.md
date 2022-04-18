# Conceptual a Lógico
1. Reemplazar jerarquía de Persona por relaciones (preferiblemente sin matar a padre o hijos) - ojo: no olvidar que el ID del padre será también usado como clave primaria para los hijos (unirlo en el diagrama)
2. Reemplazar vector de teléfonos por relación "contacto" + entidad Teléfono con atributo único
3. Mover atributos de "dirección" en Sucursal directamente a esta última entidad

# Lógico a Físico
1. Relación de contacto será una tabla (N a N)
2. Relaciones de ex-jerarquía no serán tabla, habrá 1 tabla para padre y cada hijo
3. Relación "venta de cliente" no será tabla, agregar ID de Cliente en tabla de Venta
4. Relación "intervino empleado" **SÍ** será tabla, ya que es opcional a muchos
5. Relación "venta en sucursal" NO será tabla, agregar ID de Sucursal en tabla de Venta
6. Relación de trabaja será una tabla (N a N), con fecha de inicio formando parte de la PK
7. Relación de "contiene producto" será una tabla (N a N), con PK formada solo por los IDs