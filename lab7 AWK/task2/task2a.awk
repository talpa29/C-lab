#! /usr/bin/awk -f

BEGIN {
    FS = ","
}

{
    if ( $4 == "standard") {
        M_total = M_total + $6
        R_total = R_total + $7
        W_total = W_total + $8
        total = total + 1
    }

}

END {
    printf "Math avg: %d\n" , M_total/total
    printf "Read avg: %d\n" , R_total/total
    printf "Write avg: %d\n" , W_total/total

}