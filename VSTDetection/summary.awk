BEGIN {
    ORS ="\t"
    cnt = 0;
    #print "Program\t";
    print "software_vt\t";
}

{
    if ($1 ~ /\./ && cnt > 4) {
        print $2;
#        print $1;
    }
    else {
        if ($1 ~ /\./)
            cnt += 1;
    }
}

END {
     print "1\n";
}
