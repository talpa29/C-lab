#! /usr/bin/awk -f

BEGIN {
    FS = ","
}

{
    if ( $3 == "bachelor's degree") {
        total = total + 1
    }

}

END {
    printf "students whose parents have ‘bachelor's degree: %d\n" , total
}