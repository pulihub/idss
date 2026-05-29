# import the sqlite3 module, required to interact with an sqlite database
import calendar
import datetime
import random
import sqlite3
import string
import subprocess

# import the sys module if you need - besides other things -
# to process command line arguments
import sys
import time
from sqlite3 import Error

from faker import Faker


def openDatabaseConnection(databaseFile):
    """open a connection to the Sqlite database databaseFile"""
    con = None
    try:
        con = sqlite3.connect(databaseFile)
    except Error as e:
        print(e)

    # print("The database connection is now open\n")
    return con


def closeDatabaseConnection(con):
    """close the connection con"""
    if con is not None:
        # close the cursor
        # print("\nClose the cursor\n")
        con.cursor().close()
        # close the connection to the database
        # print("Close the database connection\n")
        con.close()


def executeSelectQuery(con, sql):
    """execute the sql query using the connection con"""
    if con is not None:
        # create a cursor object
        # required to execute SQL queries and retrieve the results
        # print("Create a cursor object\n")
        cur = con.cursor()
        # now execute a SELECT query and assign the result to res
        # the fetchall() method returns all resulting rows
        # print("Retrieving data\n")
        res = cur.execute(sql)
        # print(res.fetchall())
    else:
        print("The database connection is not working")


def insertData(con, sql, params):
    """execute the sql to insert data, using the connection con"""
    if con is not None:
        # create a cursor object
        # required to execute SQL queries and retrieve the results
        # print("Create a cursor object\n")
        cur = con.cursor()
        # now execute an INSERT command
        cur.executemany(sql, params)
        con.commit()
    else:
        print("The database connection is not working")


def main():
    fake = Faker()
    current_GMT = time.time()
    Faker.seed(current_GMT)
    numargs = len(sys.argv)
    if numargs == 2:
        records = int(sys.argv[1])
        filename = f"/usr/local/server/idss_db"
        connection = openDatabaseConnection(filename)
        if connection is not None:
            # generate fake records and ingest the database
            for record in range(1, records + 1, 1):
                first = fake.first_name()
                last = fake.last_name()
                uid = fake.unique.random_int(min=1, max=2000000000)
                # uid = (server - 1) * 10 + record
                contract = fake.unique.random_int(min=10000, max=2000000000)
                # contract = (servers * records) + 1 - uid
                power = fake.random_digit_above_two()
                timestamp = fake.date_time().timestamp()
                measurement = power * random.random()
                user_data = [(first, last, uid, contract, power)]
                user_sql = "INSERT INTO tb_user VALUES(?,?,?,?,?)"
                insertData(connection, user_sql, user_data)
                consumption_data = [(timestamp, measurement, uid)]
                consumption_sql = "INSERT INTO tb_consumption VALUES(?,?,?)"
                insertData(connection, consumption_sql, consumption_data)
        else:
            print("The database connection is not working")

        closeDatabaseConnection(connection)

    else:
        print(
            "Error: number of servers and/or number of records not available on the command line"
        )


if __name__ == "__main__":
    main()
