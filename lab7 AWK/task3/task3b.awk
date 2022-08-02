#! /usr/bin/awk -f

BEGIN {
    FS = ","
}

{
   if( $0 ~ /fun|fan/)
        t = t + 1
}

END {
    print "The number of poem that have fun or fan is : ", t
}