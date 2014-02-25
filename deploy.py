#!/usr/bin/python

import sys, os

def deploy(user, prog):
    f = open("hosts.txt", "r")
    hosts = f.readlines()
    f.close()

    for host in hosts:
        host = host[:-1]
        if host == "localhost":
            continue
        print "Target: %s" % host
        os.system( "scp %s %s@%s:/home/%s" % (prog, user, host, user) )
        os.system( "ssh %s@%s mpicc %s -o %s" % ( user, host, prog, prog.replace(".c", "")  ) )
    
    os.system( "mpicc %s -o %s" % ( prog, prog.replace(".c", "")  ) )

if __name__=="__main__":
    user = sys.argv[1]
    prog = sys.argv[2]
    deploy(user, prog)
