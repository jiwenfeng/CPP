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
    <?php $this->load->view('nav', TRUE); ?>

<div class="panel panel-default">
<div class="panel-heading">服务器信息:</div>
<div class="panel-body">

<div class="input-group">
<input type="text" class="form-control" id="id" placeholder="id">
<span class="input-group-addon" id="sizing-addon1">服务器ID</span>
</div>

<div class="input-group">
<input type="text" class="form-control" id="type" placeholder="type">
<span class="input-group-addon" id="sizing-addon1">服务器平台</span>
</div>

<div class="input-group">
<input type="text" class="form-control" id="name" placeholder="name">
<span class="input-group-addon" id="sizing-addon1">服务器名</span>
</div>

<div class="input-group">
<input type="text" class="form-control" id="ip" placeholder="ip">
<span class="input-group-addon" id="sizing-addon1">服务器地址</span>
</div>

<div class="input-group">
<input type="text" class="form-control" id="port" placeholder="port">
<span class="input-group-addon" id="sizing-addon1">游戏外部端口</span>
</div>

<div class="input-group">
<input type="text" class="form-control" id="inner_port" placeholder="inner_port">
<span class="input-group-addon" id="sizing-addon1">管理内部端口</span>
</div>

<div class="input-group">
<input type="text" class="form-control" id="status" placeholder="status">
<span class="input-group-addon" id="sizing-addon1">状态</span>
</div>

<div class="input-group">
<input type="text" class="form-control" id="bulletin_board" placeholder="bulletin_board">
<span class="input-group-addon" id="sizing-addon1">公告</span>
</div>

</div>
</div>

<center>
    <button type="button" class="btn btn-info" id="add">
    <i class="glyphicon glyphicon-user"></i>
    增加
</button>
<button type="button" class="btn btn-info" id="reset">
    <i class="glyphicon glyphicon-plane"></i>
    重置
</button>
</center>

<div class="modal fade" id="helpModal">
<div class="modal-dialog">
<div class="modal-content">
<div class="modal-header">
呵呵
</div>
<div class="modal-body">
还以为有帮助
</div>
<div class="modal-footer">
呵呵
</div>
</div>
</div>
</div>

</body>

<script>

$(document).ready(function() {
    $("#help").click(function() {
        $('#helpModal').modal();
        $('#helpModal').on('show.bs.modal', function() {
        });
    });

    $("#add").click(function () {
        var id            = document.getElementById("id"   ).value,
            name          = document.getElementById("name"   ).value,
            type          = document.getElementById("type"   ).value,
            ip            = document.getElementById("ip"    ).value,
            port          = document.getElementById("port"    ).value, 
            inner_port    = document.getElementById("inner_port"    ).value, 
            status        = document.getElementById("status"    ).value, 
            bulletin_board= document.getElementById("bulletin_board"    ).value, 
            url           = "index.php?/bb/add_svr";

        $.post(
            url,
            {
                'id': id,
                'name': name,
                'type'   : type,
                'ip'    : ip,
                'port': port,
                'inner_port': inner_port,
                'status': status,
                'bulletin_board': bulletin_board,
            },
            function (data, status) {
                var info = '';
                if (data.ret != 0) {
                    info = data.info;
                } else {
                    info = "成功";
                }

                alert(info + " " + JSON.stringify(data));
                //window.location.href = 'index.php?/bb/show';
            });
    });

    $("#reset").click(function () {
        //var name       = document.getElementById("name"   ).value,
            //psw        = document.getElementById("psw"    ).value, 
            //url        = "index.php?/login/signout";

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
