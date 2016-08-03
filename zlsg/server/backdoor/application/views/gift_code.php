<!DOCTYPE html>
<html lang="zh-cn">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>礼包管理器</title>

    <link rel="stylesheet" href="http://apps.bdimg.com/libs/jqueryui/1.10.4/css/jquery-ui.min.css">

    <!-- Bootstrap -->
    <link href="http://cdn.bootcss.com/bootstrap/3.3.2/css/bootstrap.min.css" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-table/1.8.1/bootstrap-table.css" >
    <link rel="stylesheet" href="http://rawgit.com/vitalets/x-editable/master/dist/bootstrap3-editable/css/bootstrap-editable.css">

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
      <script src="http://cdn.bootcss.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="http://cdn.bootcss.com/respond.js/1.4.2/respond.min.js"></script>
      <![endif]-->

      <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
      <script src="http://cdn.bootcss.com/jquery/1.11.2/jquery.min.js"></script>
      <!-- Include all compiled plugins (below), or include individual files as needed -->
      <script src="http://cdn.bootcss.com/bootstrap/3.3.2/js/bootstrap.min.js"></script>
      <script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-table/1.8.1/bootstrap-table.js"></script>
      <script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-table/1.8.1/extensions/export/bootstrap-table-export.js"></script>
      <script src="http://rawgit.com/hhurz/tableExport.jquery.plugin/master/tableExport.js"></script>
      <script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-table/1.8.1/extensions/editable/bootstrap-table-editable.js"></script>
      <script src="http://rawgit.com/vitalets/x-editable/master/dist/bootstrap3-editable/js/bootstrap-editable.js"></script>

  </head>

  <body>
    <nav class="navbar navbar-default navbar-inverse">
        <div class="container-fluid">
            <!-- Brand and toggle get grouped for better mobile display -->
            <div class="navbar-header">
                <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="#">礼包管理器</a>
            </div>
            <!-- Collect the nav links, forms, and other content for toggling -->
            <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
                <ul class="nav navbar-nav">
                    <li><a href="index.php?/gift">礼包生成器</a></li>
                    <li><a data-toggle="modal" href="#helpModal">Help</a></li>
                </ul>
            </div>
            <!-- /.navbar-collapse -->
        </div>
        <!-- /.container-fluid -->
    </nav>

    <div class="row">
      <div class="col-md-3">
        <div class="panel panel-default">
            <div class="panel-heading">礼包列表</div>
            <div class="panel-body"> 
                <div id="toolbar-gift">
                <button id="btn-add" class="btn btn-primary" >
                    <i class="glyphicon glyphicon-plus"></i> Add
                </button>
            </div>
                <table id="gift" class="table table-striped"
                data-toolbar="#toolbar-gift"
                data-search="true"
                data-show-refresh="true"
                data-show-export="true"
                data-pagination="true"
                data-page-list="[10, 25, 50, 100, ALL]"
                >
            </table>
            <!--<ul class="list-group">
                <?php 
                foreach($gifts as $gift) {
                    echo "<li data-id=\"" . $gift['id'] . "\" class=\"gift-name list-group-item\">" . $gift['name'] . "<span class=\"badge\">" . $gift['num'] . "</span>"; 
                }
                ?>
            </li>
        </ul>-->
    </div> 
    
</div>
</div>
<div class="col-md-9">
    <div class="panel panel-default">
        <div class="panel-heading">礼包内容</div>
        <div class="panel-body"> 
            <div id="toolbar">
                <button id="remove" class="btn btn-danger" disabled>
                    <i class="glyphicon glyphicon-remove"></i> Delete
                </button>
                <button id="btn-modify" class="btn btn-info" disabled> 
                    <i class="glyphicon glyphicon-wrench"></i> Modify 
                </button>
            </div>
            <table id="code" class="table table-striped"
            data-toolbar="#toolbar"
            data-search="true"
            data-show-refresh="true"
            data-show-toggle="true"
            data-show-columns="true"
            data-show-export="true"
            data-pagination="true"
            data-page-list="[10, 25, 50, 100, ALL]"
            >
            <thead>
                <tr>
                    <th>激活码</th>
                    <th>状态</th>
                    <th>使用者ID</th>
                    <th>使用日期</th>
                </tr>
            </thead>
        </table>
    </div>
</div>
</div>
</div>

<div class="modal fade" id="helpModal">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        求人不如求己
    </div>
    <div class="modal-body">
        随便点一点
    </div>
    <div class="modal-footer">
        点错就完蛋
    </div>
</div>
</div>
</div>

<script>
var $table = $('#code'),
$remove = $('#remove'),
$modify = $('#btn-modify'),
$add    = $('#btn-add'),
id = 0;

$(document).ready(function() {
    url = 'index.php?/gift/codesjson?id=',
    col = [{field: 'code', title: '激活码'}, 
    {field: 'state', title: '状态'},  
    {field: 'role_id', title: '生成者ID'},  
    {field: 'use_time', title: '生成日期'},
    {field: 'server', title: '服务器'} 
    ],
    gift_url = "index.php?/gift/giftsjson?id=0",
    gift_col = [
    {
        field: 'name', 
        title: '礼包名'
    },
    /*{
        field: 'num', 
        title: '码总数', 
        align: 'center',
        valign: 'middle',
    },*/
    ];

    $(".gift-name").click(function(){
        id = $(this).data('id');
        console.log(id);
        $.getJSON(url + id, function(result) {
            $('#code').bootstrapTable('load', result);
        });

        $remove.prop('disabled', false);
    });

    $('#gift').bootstrapTable({
        url: gift_url, columns: gift_col
    });

    $('#gift').on('click-row.bs.table', function (e, row, element) {
        id = row.id;
        $.getJSON(url + id, function(result) {
            $('#code').bootstrapTable('load', result);
        });

        $remove.prop('disabled', false);
        $modify.prop('disabled', false);
    });

    $('#code').bootstrapTable({
        url: url + id, columns: col
    });

    $remove.click(function () {
        $.post("index.php?/gift/del", 
        {
            'id': id,
        },
        function (data, status) {
            /*$.getJSON(gift_url, function(result) {
                $('#gift').bootstrapTable('load', result);
            });*/
        alert(data.info);
        if (data.ret == 0) {
            $('#gift').bootstrapTable('remove', {field:'id', values:[id]});
            $('#code').bootstrapTable('removeAll');
        }
                  
        })});

    $modify.click(function () {
        window.location.href = 'index.php?/gift?opt=1&id=' + id;
        });

    $add.click(function () {
        window.location.href = 'index.php?/gift';
    });
});

function getIdSelections() {
    return $.map($table.bootstrapTable('getSelections'), function (row) {
        return row.id
    });
}

</script>

</body>
</html>
