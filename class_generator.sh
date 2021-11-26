#!/bin/bash
#                                                      :::      ::::::::
# auto_class.sh                                      :+:      :+:    :+:
#                                                  +:+ +:+         +:+
# By: sgah <sgah@student.42.fr>                  +#+  +:+       +#+
#                                              +#+#+#+#+#+   +#+
#   Created: 2021/05/12 16:42:04 by sgah            #+#    #+#
#                                                  ###   ########.fr
#
# -This script will create a header file protoyped *name*.hpp
#  which includes a canonical class named *name* and a cpp file
#  prototyped *name*.cpp
#     Accepts one parameter: *name* of the class
#     Usage: ./auto_class *name*


NAME="$1.hpp";

if test -f $NAME; then
        echo "$NAME already exist";
else
        vi +Stdheader $NAME +12d +wq;
        echo -n "#ifndef " >> $NAME;
        python -c "print('$1_h'.upper())" >> $NAME;
        echo -n "# define " >> $NAME;
        python -c "print('$1_h\n'.upper())" >> $NAME;
        printf "# include <iostream>\n\n" >> $NAME
        echo "class $1 {" >> $NAME;
        printf "\n\tpublic:\n\n" >> $NAME;
        printf "\t\t$1(void);\n\n" >> $NAME;
        printf "\t\t$1($1 const & src);\n" >> $NAME;
        printf "\t\t~$1(void);\n\n" >> $NAME;
        printf "\t\t$1 &\toperator=(const $1 &rhs);\n\n" >> $NAME;
        printf "\n\tprivate:\n\n" >> $NAME;
        printf "};\n" >> $NAME;
        printf "\n\n#endif" >> $NAME;
fi

NAME="$1.cpp";

if test -f $NAME; then
        echo "$NAME already exist";
else
        vi +Stdheader $NAME +12d +wq;
        printf "#include \"$1.hpp\"\n\n" >> $NAME;
        printf "$1::$1(void) {}\n\n" >> $NAME;
        printf "$1::$1($1 const & src)\n" >> $NAME;
        printf "{\n\t*this = src;\n}\n\n" >> $NAME;
        printf "$1::~$1(void) {}\n\n" >> $NAME;

fi
