# Instalamos Spark para Python
!pip install pyspark

import os

# Instalamos Java SDK 8
!apt-get install -y openjdk-8-jdk -qq > /dev/null      
!echo $(/usr/libexec/java_home -v 1.8)

#set environment variable
os.environ["JAVA_HOME"] = "/usr/lib/jvm/java-8-openjdk-amd64"     
!echo 2 | update-alternatives --config java