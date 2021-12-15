#!/bin/bash

##########################
## USE : sh test_cgi.sh ##
##########################

## EASY ENV VAR
export SERVER_NAME="127.0.0.1"
export SERVER_PORT="8080"
export SERVER_PROTOCOL="HTTP/1.1"
export SERVER_SOFTWARE="Webserv/1.1"
export GATEWAY_INTERFACE="CGI/1.1"

# export REQUEST_URI="/PHP_TEST/test2.php"
# export SCRIPT_NAME="/PHP_TEST/test2.php"
# export SCRIPT_NAME="test2.php"

##### MANDATORY FOR PHP TEST  #####
export REDIRECT_STATUS="200"

##### TEST #####
# SCRIPT_FILENAME : The path to the script executed by the cgi program
# export SCRIPT_FILENAME="/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/PHP_TEST/test.php"
# export REQUEST_METHOD="GET"

# export CONTENT_TYPE="text/php"

##### TEST 2 #####
# # SCRIPT_FILENAME : The path to the script executed by the cgi program
# export SCRIPT_FILENAME="/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/PHP_TEST/test2.php"
# export REQUEST_METHOD="GET"

# export CONTENT_TYPE="text/php"

##### SIMPLE FORM TEST #####
# export QUERY_STRING="name=John+Doe&age=42"
# # SCRIPT_FILENAME : The path to the script executed by the cgi program
# export SCRIPT_FILENAME="/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/PHP_TEST/script.php"
# export CONTENT_TYPE="text/php"

##### ENV #####
# # SCRIPT_FILENAME : The path to the script executed by the cgi program
# export SCRIPT_FILENAME="/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/PHP_TEST/env.php"
# export REQUEST_METHOD="GET"
# export CONTENT_TYPE="text/php"

##### POST #####
# # SCRIPT_FILENAME : The path to the script executed by the cgi program
export SCRIPT_FILENAME="/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/PHP_TEST/post.php"
export REQUEST_METHOD="POST"
export CONTENT_TYPE="application/x-www-form-urlencoded"
# export CONTENT_LENGTH="6"
# echo "test=1" | php-cgi

export CONTENT_LENGTH="237"
echo "------WebKitFormBoundaryzUE7XZOXwW6vBBjZ\
Content-Disposition: form-data; name=\"name\"\
\
John Doe\
------WebKitFormBoundaryzUE7XZOXwW6vBBjZ\
Content-Disposition: form-data; name=\"Age\"\
\
42\
------WebKitFormBoundaryzUE7XZOXwW6vBBjZ--" | php-cgi

# /mnt/nfs/homes/vdescham/Documents/42-webserv/cgi/linux_phpcgi
