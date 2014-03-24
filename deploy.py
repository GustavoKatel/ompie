#!/usr/bin/python

import sys, os

def deploy(user, prog):
    f = open("hosts.txt", "r")
    hosts = f.readlines()
    f.close()

    for host in hosts:
        host = host[:-1]

        if " " in host:
            host = host.split(" ")[0]
            host = host.strip()

        if host == "localhost":
            continue

        print "Target: %s" % host
        os.system( "ssh %s@%s \"cd ompie && git pull origin master && make\" " % ( user, host ) )
    

if __name__=="__main__":

    if len(sys.argv)<3:
        print "Usage: python deploy.py USER PROG_NAME"
        sys.exit(1)

    user = sys.argv[1]
    prog = sys.argv[2]

    deploy(user, prog)
