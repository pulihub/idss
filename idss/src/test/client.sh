#! /bin/bash

until [ ]; do

./idss-search-client-sql -h $1 -p $2 & ./idss-search-client-sql -h $1 -p $2 & ./idss-search-client-sql -h $1 -p $2 & ./idss-search-client-sql -h $1 -p $2 & ./idss-search-client-sql -h $1 -p $2

./idss-search-client-sql -h $1 -p $2 -f "select * from tb_host;" & ./idss-search-client-sql -h $1 -p $2 -f "select * from tb_host;" & ./idss-search-client-sql -h $1 -p $2 -f "select * from tb_host;" & ./idss-search-client-sql -h $1 -p $2 -f "select * from tb_host;"

./idss-search-client-sql -h $1 -p $2 -f "select * fro" & ./idss-search-client-sql -h $1 -p $2 -f "select * fro" & ./idss-search-client-sql -h $1 -p $2 -f "select * fro" & ./idss-search-client-sql -h $1 -p $2 -f "select * fro" & ./idss-search-client-sql -h $1 -p $2 -f "select * fro"

done
