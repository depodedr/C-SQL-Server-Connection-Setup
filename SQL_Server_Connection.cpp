// SQL_Server_Connection.cpp : This file contains code in order to connect to a SQL Server Instance 
// Wriiten by: Dylan DePodesta 
// Date: 6/13/2019
// Version: 1.0.0

#include <iostream>
#include <cstdio>
#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>

using namespace std; 

int main()
{
    // Definitions

	constexpr auto SQL_RESULT_LEN = 240;
	constexpr auto SQL_RETURN_CODE_LEN = 2048;

	// Define handles and variables 

	SQLHANDLE sqlConnHandle;
	SQLHANDLE sqlStmtHandle; 
	SQLHANDLE sqlEnvHandle; 
	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];

	// Initializations

	sqlConnHandle = NULL;
	sqlStmtHandle = NULL;

	// Allocations

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
	{
		goto COMPLETED;
	}

	if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
	{
		goto COMPLETED;
	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
	{
		goto COMPLETED;
	}

	// Outputs 

	cout << "Attemtping to connect to SQL Server..." << endl;

	switch (SQLDriverConnectW(sqlConnHandle,
		NULL,
		(SQLWCHAR*)L"DRIVER={SQL Server}; SERVER=localhost, 1433; Database=master; UID=sa; PWD=ATSAutomation1298;",
		SQL_NTS,
		retconstring,
		1024,
		NULL,
		SQL_DRIVER_NOPROMPT))

	{
	case SQL_SUCCESS:
	{
		cout << "Successfully connected to SQL Server" << endl;
		break;
	}

	case SQL_SUCCESS_WITH_INFO:
	{
		cout << "Successfully connected to SQL Server" << endl;
		break;
	}

	case SQL_INVALID_HANDLE:
	{
		cout << "Could not connect to SQL Server" << endl;
		goto COMPLETED;
	}

	case SQL_ERROR:
	{
		cout << "Could not connect to SQL Server" << endl;
		goto COMPLETED;
	}

	default:
		break;
	}

	// if there is a problem then exit application

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
	{
		goto COMPLETED;

		// Output

		cout << endl << "Executing T-SQL query..." << endl;
	}

	// If there is a problem executing the query then exit the program, else display result

	if (SQL_SUCCESS != SQLExecDirectW(sqlStmtHandle, (SQLWCHAR*)L"SELECT @@VERSION", SQL_NTS))
	{
		cout << "Error querying SQL Server" << endl;

		goto COMPLETED;
	}

	else {
		// Declare output variable and pointer 

		SQLCHAR sqlVersion[SQL_RESULT_LEN];
		SQLINTEGER ptrSqlVersion;

		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);

			// Display Query Result

			cout << endl << "Query Result: " << sqlVersion << endl;
		}

	}
	
	// Close connection and free resources 
	
	COMPLETED:
		{
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			SQLDisconnect(sqlConnHandle);
			SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
			SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

			// Pause the console window - exit when key is pressed

			cout << endl << "Press any key to exit...";
			(void)getchar();
		}
}


