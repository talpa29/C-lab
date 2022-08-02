#! /usr/bin/awk -f

BEGIN {
    FS = ","
}

{
    if ($1 > 48 && $1 < 70) {
        print "--------------------"
        print "Actor Name:" , $4
        print "Movie Name:", $5

    }
}