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

        if len(prog.split("/"))>0:
            prog_name = prog.split("/")[-1:][0]
        else:
            prog_name = prog
        os.system( "ssh %s@%s mpicc %s -o %s" % ( user, host, prog_name, prog_name.replace(".c", ".out")  ) )
    

if __name__=="__main__":

    if len(sys.argv)<3:
        print "Usage: python deploy.py USER PROG_NAME"
        sys.exit(1)

    user = sys.argv[1]
    prog = sys.argv[2]

    deploy(user, prog)
