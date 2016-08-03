 <!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>后台登陆</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>
  
</head>

<body>
<?php if ($is_login) {$this->load->view('nav', TRUE);} ?>

<style type="text/css">
.span-align-login { 
    width: 120px;
}
</style>

<div class="container">
    <div class="row">
        <div class="col-md-4 col-md-offset-4">
<?php
if (!$is_login) {
echo '<div class="panel panel-default">
<div class="panel-heading">登陆:</div>
<div class="panel-body">

<div class="input-group">
<span class="span-align-login input-group-addon " id="sizing-addon1" name="user">用户:</span>
<input type="text" class="form-control" id="name" placeholder="name">
</div>

<div class="input-group">
<span class="span-align-login input-group-addon " id="sizing-addon1" name="password">密码:</span>
<input type="password" class="form-control" id="psw" placeholder="password">
</div>

</div>
</div>

<center>
    <button type="button" class="btn btn-info" id="signin">
    <i class="glyphicon glyphicon-user"></i>
    登陆
</button>'

;
} else {
    echo "<center> 玩开心 $name ";
}
?>

<button type="button" class="btn btn-info" id="signout">
    <i class="glyphicon glyphicon-plane"></i>
    退出
</button>
</center>

</div>
</div>
</div>

<?php $this->load->view('help'); ?>

</body>

<script>

$(document).ready(function() {
    $("#help").click(function() {
        $('#helpModal').modal();
        $('#helpModal').on('show.bs.modal', function() {
        });
    });

    $("#signin").click(function () {
        var name       = document.getElementById("name"   ).value,
            psw        = document.getElementById("psw"    ).value, 
            url        = "index.php?/login/signin";

        $.post(
            url,
            {
                'name'   : name,
                'psw'    : psw,
            },
            function (data, status) {
                var info = '';

                if (data.ret != 0) {
                    info = data.info;
                    alert(info + " " + JSON.stringify(data));    
                } else {
                    info = "成功";
                    location.reload(true);
                    //window.location.href = 'index.php?/login';
                }

                //alert(info + " " + JSON.stringify(data));    
            });
    });

    $("#signout").click(function () {
        var //name       = document.getElementById("name"   ).value,
            //psw        = document.getElementById("psw"    ).value, 
            url        = "index.php?/login/signout";

        $.post(
            url,
            {
               // 'name'   : name,
                //'psw'    : psw,
            },
            function (data, status) {
                var info = '';
                if (data.ret != 0) {
                    info = data.info;
                } else {
                    info = "成功";
                }

                alert(info + " " + JSON.stringify(data));
                window.location.href = 'index.php?/login';
            });
    });
});

</script>
</html>
