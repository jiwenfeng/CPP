backdoor 游戏后端信息接口
=========================

功能介绍
--------

> 1.	游戏服务器列表读写
> 2.	游戏客户端程序资源版本信息读写
> 3.	游戏服务器数据读写

实现程序语言与框架
------------------

### 所用语言

#### PHP

### 开发所用框架

#### nginx

#### mysql

#### php

#### CodeIgniter

> CodeIgniter 是一个小巧但功能强大的 PHP 框架, 作为一个简单而“优雅”的工具包， 它可以为 PHP 程序员建立功能完善的 Web 应用程序。
>
> 开发中所使用版本为2.2-stable
>
> -	[官方地址](http://www.codeigniter.com/)
> -	[中文网站](http://www.codeigniter.org.cn/)

配置与功能扩展
--------------

### 数据库配置

> 配置文件地址为:`./application/config/database.php`
>
> 内容格式:
>
> ```php
> $db['default']['hostname'] = 'hostname';
> $db['default']['username'] = 'username';
> $db['default']['password'] = 'password';
> $db['default']['database'] = 'database';
> $db['default']['dbdriver'] = 'mysql';
> $db['default']['dbprefix'] = '';
> $db['default']['pconnect'] = TRUE;
> $db['default']['db_debug'] = TRUE;
> $db['default']['cache_on'] = FALSE;
> $db['default']['cachedir'] = '';
> $db['default']['char_set'] = 'utf8';
> $db['default']['dbcollat'] = 'utf8_general_ci';
> $db['default']['swap_pre'] = '';
> $db['default']['autoinit'] = TRUE;
> $db['default']['stricton'] = FALSE;
> ```
>
> 一般情况只需要修改对应的`hostname` `username` `password` `database`

### 游戏服务端配置

### 添加新功能

> 功能文件路径为:`./application/controllers/`
>
> 可参考示例模板:`template.php`
