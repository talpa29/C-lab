#! /usr/bin/awk -f

BEGIN {
    FS = ","
}

{
    print "--------------------"
    print "Actor Name:" , $4
    print "Movie Name:", $5

}