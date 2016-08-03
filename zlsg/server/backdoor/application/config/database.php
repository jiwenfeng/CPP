<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');
/*
| -------------------------------------------------------------------
| DATABASE CONNECTIVITY SETTINGS
| -------------------------------------------------------------------
| This file will contain the settings needed to access your database.
|
| For complete instructions please consult the 'Database Connection'
| page of the User Guide.
|
| -------------------------------------------------------------------
| EXPLANATION OF VARIABLES
| -------------------------------------------------------------------
|
|	['hostname'] The hostname of your database server.
|	['username'] The username used to connect to the database
|	['password'] The password used to connect to the database
|	['database'] The name of the database you want to connect to
|	['dbdriver'] The database type. ie: mysql.  Currently supported:
				 mysql, mysqli, postgre, odbc, mssql, sqlite, oci8
|	['dbprefix'] You can add an optional prefix, which will be added
|				 to the table name when using the  Active Record class
|	['pconnect'] TRUE/FALSE - Whether to use a persistent connection
|	['db_debug'] TRUE/FALSE - Whether database errors should be displayed.
|	['cache_on'] TRUE/FALSE - Enables/disables query caching
|	['cachedir'] The path to the folder where cache files should be stored
|	['char_set'] The character set used in communicating with the database
|	['dbcollat'] The character collation used in communicating with the database
|				 NOTE: For MySQL and MySQLi databases, this setting is only used
| 				 as a backup if your server is running PHP < 5.2.3 or MySQL < 5.0.7
|				 (and in table creation queries made with DB Forge).
| 				 There is an incompatibility in PHP with mysql_real_escape_string() which
| 				 can make your site vulnerable to SQL injection if you are using a
| 				 multi-byte character set and are running versions lower than these.
| 				 Sites using Latin-1 or UTF-8 database character set and collation are unaffected.
|	['swap_pre'] A default table prefix that should be swapped with the dbprefix
|	['autoinit'] Whether or not to automatically initialize the database.
|	['stricton'] TRUE/FALSE - forces 'Strict Mode' connections
|							- good for ensuring strict SQL while developing
|
| The $active_group variable lets you choose which connection group to
| make active.  By default there is only one group (the 'default' group).
|
| The $active_record variables lets you determine whether or not to load
| the active record class
*/

$active_group = 'default';
$active_record = TRUE;

$db['default']['hostname'] = 'localhost';
$db['default']['username'] = 'abel';
$db['default']['password'] = '0402';
$db['default']['database'] = 'updateserver';
$db['default']['dbdriver'] = 'mysql';
$db['default']['dbprefix'] = '';
$db['default']['pconnect'] = TRUE;
$db['default']['db_debug'] = TRUE;
$db['default']['cache_on'] = FALSE;
$db['default']['cachedir'] = '';
$db['default']['char_set'] = 'utf8';
$db['default']['dbcollat'] = 'utf8_general_ci';
$db['default']['swap_pre'] = '';
$db['default']['autoinit'] = TRUE;
$db['default']['stricton'] = FALSE;

$db['game']['hostname'] = 'localhost';
$db['game']['username'] = 'abel';
$db['game']['password'] = '0402';
$db['game']['database'] = 'designer';
$db['game']['dbdriver'] = 'mysql';
$db['game']['dbprefix'] = '';
$db['game']['pconnect'] = TRUE;
$db['game']['db_debug'] = TRUE;
$db['game']['cache_on'] = FALSE;
$db['game']['cachedir'] = '';
$db['game']['char_set'] = 'utf8';
$db['game']['dbcollat'] = 'utf8_general_ci';
$db['game']['swap_pre'] = '';
$db['game']['autoinit'] = TRUE;
$db['game']['stricton'] = FALSE;

$db['backdoor']['hostname'] = 'localhost';
$db['backdoor']['username'] = 'abel';
$db['backdoor']['password'] = '0402';
$db['backdoor']['database'] = 'designer';
$db['backdoor']['dbdriver'] = 'mysql';
$db['backdoor']['dbprefix'] = '';
$db['backdoor']['pconnect'] = TRUE;
$db['backdoor']['db_debug'] = TRUE;
$db['backdoor']['cache_on'] = FALSE;
$db['backdoor']['cachedir'] = '';
$db['backdoor']['char_set'] = 'utf8';
$db['backdoor']['dbcollat'] = 'utf8_general_ci';
$db['backdoor']['swap_pre'] = '';
$db['backdoor']['autoinit'] = TRUE;
$db['backdoor']['stricton'] = FALSE;

$db['server_list']['hostname'] = 'localhost';
$db['server_list']['username'] = 'abel';
$db['server_list']['password'] = '0402';
$db['server_list']['database'] = 'server_list';
$db['server_list']['dbdriver'] = 'mysql';
$db['server_list']['dbprefix'] = '';
$db['server_list']['pconnect'] = FALSE;
$db['server_list']['db_debug'] = TRUE;
$db['server_list']['cache_on'] = FALSE;
$db['server_list']['cachedir'] = '';
$db['server_list']['char_set'] = 'utf8';
$db['server_list']['dbcollat'] = 'utf8_general_ci';
$db['server_list']['swap_pre'] = '';
$db['server_list']['autoinit'] = TRUE;
$db['server_list']['stricton'] = FALSE;

$db['filesmgr']['hostname'] = 'localhost';
$db['filesmgr']['username'] = 'abel';
$db['filesmgr']['password'] = '0402';
$db['filesmgr']['database'] = 'updateserver';
$db['filesmgr']['dbdriver'] = 'mysql';
$db['filesmgr']['dbprefix'] = '';
$db['filesmgr']['pconnect'] = FALSE;
$db['filesmgr']['db_debug'] = TRUE;
$db['filesmgr']['cache_on'] = FALSE;
$db['filesmgr']['cachedir'] = '';
$db['filesmgr']['char_set'] = 'utf8';
$db['filesmgr']['dbcollat'] = 'utf8_general_ci';
$db['filesmgr']['swap_pre'] = '';
$db['filesmgr']['autoinit'] = TRUE;
$db['filesmgr']['stricton'] = FALSE;

$db['gift']['hostname'] = 'localhost';
$db['gift']['username'] = 'abel';
$db['gift']['password'] = '0402';
$db['gift']['database'] = 'gift';
$db['gift']['dbdriver'] = 'mysql';
$db['gift']['dbprefix'] = '';
$db['gift']['pconnect'] = FALSE;
$db['gift']['db_debug'] = TRUE;
$db['gift']['cache_on'] = FALSE;
$db['gift']['cachedir'] = '';
$db['gift']['char_set'] = 'utf8';
$db['gift']['dbcollat'] = 'utf8_general_ci';
$db['gift']['swap_pre'] = '';
$db['gift']['autoinit'] = TRUE;
$db['gift']['stricton'] = FALSE;

/* End of file database.php */
/* Location: ./application/config/database.php */
