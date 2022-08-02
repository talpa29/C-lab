#! /usr/bin/awk -f

BEGIN {
    FS = ","
    print "========"
    print "Success Student List"
    print "========"
}

{
    if ( $6 >= 80 && $7 >= 80 && $8 >= 80) {
        print $0
        total = total + 1
    }
}

END {
    print "The number of students :" , total
}