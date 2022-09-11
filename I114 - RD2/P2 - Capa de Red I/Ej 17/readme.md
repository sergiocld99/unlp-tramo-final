## Configure cada uno de los equipos

![esquema ej17](ej17-esquema.png)

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

* Para el tráfico desconocido: de n1 a n2, de n2 a n3, y de n3 a n1. NOTA: se indica la puerta de entrada del próximo router.
```
root@n1:/tmp/pycore.38455/n1.conf# route add default gw 10.0.1.1
root@n2:/tmp/pycore.38455/n2.conf# route add default gw 10.0.3.2
root@n3:/tmp/pycore.38455/n3.conf# route add default gw 10.0.2.1
```

## Conectividad entre las PCs

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

* De n10 a n8. Vemos que pasa por n3 > n2 > n8.
```
root@n10:/tmp/pycore.34143/n10.conf# traceroute 10.0.4.21
traceroute to 10.0.4.21 (10.0.4.21), 30 hops max, 60 byte packets
 1  10.0.5.1 (10.0.5.1)  0.035 ms  0.008 ms  0.008 ms
 2  10.0.3.1 (10.0.3.1)  0.029 ms  0.017 ms  0.016 ms
 3  10.0.4.21 (10.0.4.21)  0.029 ms  0.020 ms  0.023 ms
```

## Relevamiento

* Asi quedó para el router n1:
```
root@n1:/tmp/pycore.34143/n1.conf# route -n
Tabla de rutas IP del núcleo
Destino         Pasarela        Genmask         Indic Métric Ref    Uso Interfaz
0.0.0.0         10.0.1.1        0.0.0.0         UG    0      0        0 eth1
10.0.0.0        0.0.0.0         255.255.255.0   U     0      0        0 eth0
10.0.1.0        0.0.0.0         255.255.255.0   U     0      0        0 eth1
10.0.2.0        0.0.0.0         255.255.255.0   U     0      0        0 eth2
10.0.3.0        10.0.1.1        255.255.255.0   UG    20     0        0 eth1
10.0.4.0        10.0.1.1        255.255.255.0   UG    20     0        0 eth1
10.0.5.0        10.0.2.2        255.255.255.0   UG    20     0        0 eth2
```

* n2:
```
root@n2:/tmp/pycore.34143/n2.conf# route -n
Tabla de rutas IP del núcleo
Destino         Pasarela        Genmask         Indic Métric Ref    Uso Interfaz
0.0.0.0         10.0.3.2        0.0.0.0         UG    0      0        0 eth1
10.0.0.0        10.0.1.2        255.255.255.0   UG    20     0        0 eth0
10.0.1.0        0.0.0.0         255.255.255.0   U     0      0        0 eth0
10.0.2.0        10.0.1.2        255.255.255.0   UG    20     0        0 eth0
10.0.3.0        0.0.0.0         255.255.255.0   U     0      0        0 eth1
10.0.4.0        0.0.0.0         255.255.255.0   U     0      0        0 eth2
10.0.5.0        10.0.3.2        255.255.255.0   UG    20     0        0 eth1
```

* n3:
```
root@n3:/tmp/pycore.34143/n3.conf# route -n
Tabla de rutas IP del núcleo
Destino         Pasarela        Genmask         Indic Métric Ref    Uso Interfaz
0.0.0.0         10.0.2.1        0.0.0.0         UG    0      0        0 eth0
10.0.0.0        10.0.2.1        255.255.255.0   UG    20     0        0 eth0
10.0.1.0        10.0.2.1        255.255.255.0   UG    20     0        0 eth0
10.0.2.0        0.0.0.0         255.255.255.0   U     0      0        0 eth0
10.0.3.0        0.0.0.0         255.255.255.0   U     0      0        0 eth1
10.0.4.0        10.0.3.1        255.255.255.0   UG    20     0        0 eth1
10.0.5.0        0.0.0.0         255.255.255.0   U     0      0        0 eth2
```

* Host n6:
```
root@n6:/tmp/pycore.34143/n6.conf# route -n
Tabla de rutas IP del núcleo
Destino         Pasarela        Genmask         Indic Métric Ref    Uso Interfaz
0.0.0.0         10.0.0.1        0.0.0.0         UG    0      0        0 eth0
10.0.0.0        0.0.0.0         255.255.255.0   U     0      0        0 eth0
```

* Host n10:
```
root@n10:/tmp/pycore.34143/n10.conf# route -n
Tabla de rutas IP del núcleo
Destino         Pasarela        Genmask         Indic Métric Ref    Uso Interfaz
0.0.0.0         10.0.5.1        0.0.0.0         UG    0      0        0 eth0
10.0.5.0        0.0.0.0         255.255.255.0   U     0      0        0 eth0
```

### Respondiendo preguntas
Si la estación PC n7 le envía un ping a la estación PC n6:
1. ¿Cuál es el camino por el que viaja el requerimiento?
2. ¿Cuál es el camino por el que viaja la respuesta?

Respuesta: el requerimiento sigue el camino n7 > n2 > n1 > n6, la respuesta el opuesto n6 > n1 > n2 > n7.

```
root@n7:/tmp/pycore.34143/n7.conf# ping -nR 10.0.0.10
PING 10.0.0.10 (10.0.0.10) 56(124) bytes of data.
64 bytes from 10.0.0.10: icmp_seq=1 ttl=62 time=0.051 ms
RR: 	10.0.4.22
	10.0.1.1
	10.0.0.1
	10.0.0.10
	10.0.0.10
	10.0.1.2
	10.0.4.1
	10.0.4.22
```

## Mantenimiento de la red
Suponiendo que en el Router n1 se agregó una interfaz de red con la dirección IP 163.10.10.1/24:
1. Identifique los cambios que deberían ser necesarios al ruteo estático para
que todas las PCs puedan comunicarse con hosts en la nueva red.
2. En base a lo anterior, ¿qué puede decir respecto del mantenimiento del ruteo
en una red que tiene más de 20 routers?

Respuesta 1: agregando una PC con IP 163.10.10.20/24 y conectándola al router n1 con interfaz eth3 e IP 163.10.10.1/24 ya funcionó, probé el ping desde todos los hosts y pudieron comunicarse con el nuevo.

```
root@n1:/tmp/pycore.34143/n1.conf# route -n         
Tabla de rutas IP del núcleo
Destino         Pasarela        Genmask         Indic Métric Ref    Uso Interfaz
0.0.0.0         10.0.1.1        0.0.0.0         UG    0      0        0 eth1
10.0.0.0        0.0.0.0         255.255.255.0   U     0      0        0 eth0
10.0.1.0        0.0.0.0         255.255.255.0   U     0      0        0 eth1
10.0.2.0        0.0.0.0         255.255.255.0   U     0      0        0 eth2
10.0.3.0        10.0.1.1        255.255.255.0   UG    20     0        0 eth1
10.0.4.0        10.0.1.1        255.255.255.0   UG    20     0        0 eth1
10.0.5.0        10.0.2.2        255.255.255.0   UG    20     0        0 eth2
163.10.10.0     0.0.0.0         255.255.255.0   U     0      0        0 eth3
```

```
root@n2:/tmp/pycore.34143/n2.conf# route -n
Tabla de rutas IP del núcleo
Destino         Pasarela        Genmask         Indic Métric Ref    Uso Interfaz
0.0.0.0         10.0.3.2        0.0.0.0         UG    0      0        0 eth1
10.0.0.0        10.0.1.2        255.255.255.0   UG    20     0        0 eth0
10.0.1.0        0.0.0.0         255.255.255.0   U     0      0        0 eth0
10.0.2.0        10.0.1.2        255.255.255.0   UG    20     0        0 eth0
10.0.3.0        0.0.0.0         255.255.255.0   U     0      0        0 eth1
10.0.4.0        0.0.0.0         255.255.255.0   U     0      0        0 eth2
10.0.5.0        10.0.3.2        255.255.255.0   UG    20     0        0 eth1
163.10.10.0     10.0.1.2        255.255.255.0   UG    20     0        0 eth0
```

Respuesta 2: no correlation, no ayuda visual, no respondo.

## ICMP y RUTEO 1
Desde la PC n6, realice un ping a la dirección IP 5.5.5.5. ¿Qué indica el mensaje de error recibido?, ¿Quién lo envía?

```
root@n6:/tmp/pycore.34143/n6.conf# ping 5.5.5.5
PING 5.5.5.5 (5.5.5.5) 56(84) bytes of data.
From 10.0.0.1 icmp_seq=1 Time to live exceeded
```

Respuesta: indica TTL excedido porque se la pasó dando vueltas entre n1 > n2 > n3 > n1, por ser tráfico desconocido.

## ICMP y RUTEO 2
Desde la PC n6, realice un ping a la dirección IP 10.0.5.23. ¿Qué indica el mensaje de error recibido?, ¿Quién lo envió?

```
root@n6:/tmp/pycore.34143/n6.conf# ping 10.0.5.23
PING 10.0.5.23 (10.0.5.23) 56(84) bytes of data.
From 10.0.2.2 icmp_seq=1 Destination Host Unreachable
```

Respuesta: el mensaje indica que el destino es inalcanzable, esto es porque no existe en la red 10.0.5.0 (camino n6 > n1 > n3)

## ICMP y RUTEO 3
Provoque un loop de enrutamiento entre los routers con una
nueva red, por ejemplo la red 200.100.11.0/24 y luego desde la PC n6, realice un ping a la dirección 200.100.11.5. ¿Qué indica el mensaje de error recibido?, ¿Quién lo envía?

No entendí... ._. XD