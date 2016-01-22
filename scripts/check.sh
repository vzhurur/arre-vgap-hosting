# !/bin/bash

# Programa para comprobar si hay un nuevo turno subido al servidor web
# Y revisarlo para confirmar su valided
# (C) Javier Castellanos 2015

# Directorio del phost
phost="/home/vgap/phost"

# Directorio de la partida
partida=babylon5

# Directorio donde estan los turnos que se descargan
upload="/home/vgap/turns/uploads/$partida"

# Archivo de log
logfile="/home/vgap/phost/logs/$partida-check.log"

# Me situo en el directorio del phost, por si acaso ha pasado algo raro
cd $phost

# para evitar que falle si no hay archivos
# shopt -s nullglob

# Compruebo si hay un ficheros nuevos, y en caso afirmativo, los muevo y los reviso
# Reviso los turnos y saco el resultado a un fichero

ficheros=$upload/*.trn 
for f in $ficheros
do
  # echo "Processing $f file..."
  # take action on each file. $f store current file name
  mv $f $phost/$partida
done

if [ -n $ficheros ]
   then
      ./phost -c0 $partida | tail -18 >  $logfile
fi
