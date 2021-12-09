#!/bin/bash

##########################
## USE : sh test_cgi.sh ##
##########################

export REQUEST_METHOD="GET"
export SERVER_PROTOCOL="HTTP/1.1"
export SERVER_NAME="Webserv_POC"
export SERVER_PORT="8080"
export SERVER_SOFTWARE="Webserv/1.1"
export GATEWAY_INTERFACE="CGI/1.1"
export CONTENT_TYPE="text/php"

export SCRIPT_NAME="/mnt/nfs/homes/vdescham/Documents/42-webserv/42_testers/linux_phpcgi"
# export SCRIPT_NAME="test2.php"

##### MANDATORY FOR PHP TEST  #####
export REDIRECT_STATUS="200"

##### TEST #####
# export PATH_INFO="/test.php"
# export PATH_TRANSLATED="/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/PHP_TEST/test.php"

##### TEST 2 #####
export PATH_INFO="/test2.php"
export PATH_TRANSLATED="/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/PHP_TEST/test2.php"

/mnt/nfs/homes/vdescham/Documents/42-webserv/42_testers/linux_phpcgi
