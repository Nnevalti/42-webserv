#!/bin/bash

###########################################
## USE : ./cgi_file.cgi | sh test_cgi.sh ##
###########################################

export REQUEST_METHOD="GET"
export SERVER_PROTOCOL="HTTP/1.1"
export SERVER_NAME="Webserv_POC"
export SERVER_PORT="8080"
export SERVER_SOFTWARE="Webserv/1.1"
export GATEWAY_INTERFACE="CGI/1.1"

# export SCRIPT_NAME="/mnt/nfs/homes/vdescham/Documents/42-webserv/42_testers/ubuntu_cgi_tester"
# export SCRIPT_NAME="test2.php"

##### SIMPLE CGI TEST #####
# export PATH_INFO="/CGI_TEST/simple.cgi"
# export PATH_TRANSLATED="/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/CGI_TEST/simple.cgi"

##### DYNAMIC CGI TEST #####
# export PATH_INFO="/CGI_TEST/count.cgi"
# export PATH_TRANSLATED="/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/CGI_TEST/count.cgi"

##### SIMPLE FORM TEST #####
export QUERY_STRING="name=John+Doe"
export PATH_INFO="/CGI_TEST/simpleform.cgi"
export PATH_TRANSLATED="/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/CGI_TEST/simpleform.cgi"

/mnt/nfs/homes/vdescham/Documents/42-webserv/cgi/ubuntu_cgi_tester
