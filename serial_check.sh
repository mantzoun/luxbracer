#!/bin/bash

IFS=$'\n'

if [[ $1 == "" ]]
then
    echo no file provided
    exit 1
fi

parse_variable() {
    # no functions or variables marked as non serializable
    if egrep -q '\(|no_serial|\)' <<< "$1"
    then
        return
    fi

    sanitized=`echo $1 |sed 's/[;\*=]//g' | sed 's/^ *//g' |sed 's/  */ /g'| sed 's/<.*>//g'`
    var=`awk -F' *' '{print $2}' <<< "$sanitized"`
    VARIABLES+=($var)
}

parse_from_json() {
    if grep -v -q 'get_to' <<< "$1"
    then
        return
    fi

    var=`awk -F'"' '{print $2}' <<< "$1"`
    FROM_JSON+=($var)
}

parse_to_json() {
    if grep -v -q '"' <<< "$1"
    then
        return
    fi

    var=`awk -F'"' '{print $2}' <<< "$1"`
    TO_JSON+=($var)
}

parse_line() {
    if [[ $private == 1 ]]
    then
        parse_variable $1
    elif [[ $from_json == 1 ]]
    then
        parse_from_json $1
    elif [[ $to_json == 1 ]]
    then
        parse_to_json $1
    fi
}

check_file() {
    FILE=$1

    if grep -q from_json $FILE || grep -q to_json $FILE
    then
        lines=`egrep -v 'friend' $FILE`
        for line in $lines
        do
            if grep -q private <<< "$line"
            then
                private=1
            elif grep -q to_json <<< "$line"
            then
                if [[ $from_json == 1 ]]
                then
                    from_json=2
                fi
                to_json=1
                private=0
                check=1
            elif grep -q from_json <<< "$line"
            then
                if [[ $to_json == 1 ]]
                then
                    to_json=2
                fi
                from_json=1
                private=0
                check=1
            else
                parse_line $line
            fi
        done
    fi

    if [[ $check == 0 ]]
    then
        return
    fi

    readarray -t comparison < <(echo ${VARIABLES[@]} ${TO_JSON[@]} | tr ' ' '\n' | sort | uniq -c |grep -v '2 ')

    if [[ ${#comparison[@]} != 0 && $to_json != 0 ]]
    then
        echo serialization problems in file file $FILE
        echo ${comparison[@]}
        error=1
    fi

    readarray -t comparison < <(echo ${VARIABLES[@]} ${FROM_JSON[@]} | tr ' ' '\n' | sort | uniq -c |grep -v '2 ')

    if [[ ${#comparison[@]} != 0 && $from_json != 0 ]]
    then
        echo deserialization problems in file file $FILE
        echo ${comparison[@]}
        error=1
    fi
}

error=0

while [[ $1 != "" ]]
do
    echo "check (de)serialization in file $1"

    private=0
    public=0
    to_json=0
    from_json=0
    check=0

    VARIABLES=()
    TO_JSON=()
    FROM_JSON=()

    check_file $1
    shift
done

exit $error
