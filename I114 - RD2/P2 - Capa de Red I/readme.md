# Práctica 2 - Capa de Red (I) - IPv4/IPv6

## Ej 10 - Sumarizar 
¿Cómo se podrían sumarizar las siguientes direcciones aplicando CIDR?
- 200.10.0.0/24
- 200.10.1.0/24
- 200.10.2.0/24
- 200.10.3.0/24

Vemos que las 4 redes son consecutivas, así que vemos cuales son los bits en común (desde el más significativo)

* Con 200.10.X.X tenemos 16 bits comunes 
* Luego de 0 a 3 tenemos formato 0000 00XX
* Resultan 22 bits comunes

Luego, verificamos que no haya ninguna red de más en el grupo. 
Todo bien. Se pasan los bits XX a cero y se especifica la nueva máscara. 

- Respuesta final: Si, es 200.10.0.0/22

## Ej 11
¿Cómo quedará la sumarización si necesita agrupar las 8 primeras redes (hasta las
200.10.7.0/24)

* De 0 a 7 tenemos formato 0000 0XXX
* Resultan 21 bits comunes

Nuevamente no hay redes de más.

* Respuesta: 200.10.0.0/21

## Ej 12
Y las siguientes:
- 195.80.0.0/24
- 195.80.1.0/24
- 195.80.2.0/24

* De 0 a 2 tenemos formato 0000 00XX
* Son 22 bits comunes, PERO estamos incluyendo a 195.80.3.0

Entonces no se puede sumarizar en 1 solo grupo. 

* De 0 a 1 tenemos formato 0000 000X
* Son 23 bits comunes para las primeras 2.

Sumarizo las primeras 2 y la tercera no puedo (verdad?)

* Respuesta: 195.80.0.0/23 para las 2 primeras

## Ej 17
Carpeta compartida: punto de montaje dentro de "/home/rysa/", con automontaje y permiso habilitado mediante:

``` sudo usermod -a -G vboxsf rysa ```

Luego reiniciamos y ya está todo listo para usar.

### Asignación de direcciones IP

* En Router n1:
``` 
root@n1:/tmp/pycore.37217/n1.conf# ifconfig eth0 10.0.0.1
root@n1:/tmp/pycore.37217/n1.conf# ifconfig eth1 10.0.1.2
root@n1:/tmp/pycore.37217/n1.conf# ifconfig eth2 10.0.2.1 
```

* En Router n2:
``` 
root@n2:/tmp/pycore.37217/n2.conf# ifconfig eth0 10.0.1.1
root@n2:/tmp/pycore.37217/n2.conf# ifconfig eth1 10.0.3.1
root@n2:/tmp/pycore.37217/n2.conf# ifconfig eth2 10.0.4.1
``` 

* En Router n3:
``` 
root@n3:/tmp/pycore.37217/n3.conf# ifconfig eth0 10.0.2.2
root@n3:/tmp/pycore.37217/n3.conf# ifconfig eth1 10.0.3.2
root@n3:/tmp/pycore.37217/n3.conf# ifconfig eth2 10.0.5.1
``` 

* En las PC (n6 a n12):
``` 
root@n6:/tmp/pycore.37217/n1.conf# ifconfig eth0 10.0.0.10 
root@n7:/tmp/pycore.37217/n7.conf# ifconfig eth0 10.0.4.22
root@n8:/tmp/pycore.37217/n8.conf# ifconfig eth0 10.0.4.21
root@n9:/tmp/pycore.37217/n9.conf# ifconfig eth0 10.0.4.20
root@n10:/tmp/pycore.37217/n10.conf# ifconfig eth0 10.0.5.20
root@n11:/tmp/pycore.37217/n11.conf# ifconfig eth0 10.0.5.21
root@n12:/tmp/pycore.37217/n12.conf# ifconfig eth0 10.0.5.22
```

### Verificación de conectividad

* Del host n6 al router n1:
```
root@n6:/tmp/pycore.43267/n6.conf# ping -a 10.0.0.1 
PING 10.0.0.1 (10.0.0.1) 56(84) bytes of data.
64 bytes from 10.0.0.1: icmp_seq=1 ttl=64 time=0.058 ms
64 bytes from 10.0.0.1: icmp_seq=2 ttl=64 time=0.092 ms
64 bytes from 10.0.0.1: icmp_seq=3 ttl=64 time=0.070 ms
64 bytes from 10.0.0.1: icmp_seq=4 ttl=64 time=0.083 ms
64 bytes from 10.0.0.1: icmp_seq=5 ttl=64 time=0.050 ms
64 bytes from 10.0.0.1: icmp_seq=6 ttl=64 time=0.065 ms
64 bytes from 10.0.0.1: icmp_seq=7 ttl=64 time=0.057 ms
```

### Tablas de ruteo
* Para el tráfico desconocido: de n1 a n2, de n2 a n3, y de n3 a n1. NOTA: se indica la puerta de entrada del próximo router.
```
root@n1:/tmp/pycore.38455/n1.conf# route add default gw 10.0.1.1
root@n2:/tmp/pycore.38455/n2.conf# route add default gw 10.0.3.2
root@n3:/tmp/pycore.38455/n3.conf# route add default gw 10.0.2.1
```

### Traceroute
* De n6 a n7. Vemos que pasa por n1 > n2 > n7.
```
root@n6:/tmp/pycore.34143/n6.conf# traceroute 10.0.4.22
traceroute to 10.0.4.22 (10.0.4.22), 30 hops max, 60 byte packets
 1  10.0.0.1 (10.0.0.1)  0.032 ms  0.012 ms  0.007 ms
 2  10.0.1.1 (10.0.1.1)  0.018 ms  0.009 ms  0.009 ms
 3  10.0.4.22 (10.0.4.22)  0.024 ms  0.014 ms  0.022 ms
```

* De n6 a n10. Vemos que pasa por n1 > n3 > n10.
```
root@n6:/tmp/pycore.34143/n6.conf# traceroute 10.0.5.20
traceroute to 10.0.5.20 (10.0.5.20), 30 hops max, 60 byte packets
 1  10.0.0.1 (10.0.0.1)  0.029 ms  0.006 ms  0.006 ms
 2  10.0.2.2 (10.0.2.2)  0.020 ms  0.009 ms  0.009 ms
 3  10.0.5.20 (10.0.5.20)  0.023 ms  0.014 ms  0.013 ms
root@n6:/tmp/pycore.34143/n6.conf# 
```

* De n6 a desconocido. Vemos que da vueltas entre n1 > n2 > n3 > n1... hasta que TTL = 0. Se muestra una parte:
```
root@n6:/tmp/pycore.34143/n6.conf# traceroute 10.0.8.19
traceroute to 10.0.8.19 (10.0.8.19), 30 hops max, 60 byte packets
 1  10.0.0.1 (10.0.0.1)  0.030 ms  0.022 ms  0.009 ms
 2  10.0.1.1 (10.0.1.1)  0.023 ms  0.014 ms  0.012 ms
 3  10.0.2.2 (10.0.2.2)  0.019 ms  0.010 ms  0.010 ms
 4  10.0.0.1 (10.0.0.1)  0.010 ms  0.009 ms  0.009 ms
 5  10.0.1.1 (10.0.1.1)  0.012 ms  0.012 ms  0.012 ms
 6  10.0.2.2 (10.0.2.2)  0.013 ms  0.027 ms  0.014 ms
 7  * * *
```

## Ej 20
Los mensajes ARP, ¿son re-enviados por los routers? Justifique.

* Respuesta: NO, porque sino se repartirán fuera de una red, inundando Internet??

## Ej 24
¿Cuál sería una abreviatura correcta de 3f80:0000:0000:0a00:0000:0000:0000:0845?
- 3f80::a00::845
- 3f80::a:845
- 3f80::a00:0:0:0:845:4567
- 3f80:0:0:a00::845
- 3f8:0:0:a00::845

* Respuesta: la cuarta
