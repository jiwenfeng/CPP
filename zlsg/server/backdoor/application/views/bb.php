<!DOCTYPE html>
<html lang="zh-cn">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>服务器配置</title>

    <?php $this->load->view('css'); ?>
    <?php $this->load->view('js'); ?>

</head>

  <body>
    <?php $this->load->view('nav', TRUE); ?>

    <div class="row">
      <div class="col-md-12">
        <div class="panel panel-default">
            <div class="panel-heading">服务器列表</div>
            <div class="panel-body"> 
                <div id="toolbar-server">
                <button id="btn-add" class="btn btn-primary" >
                    <i class="glyphicon glyphicon-plus"></i> Add
                </button>
                <button id="btn-del" class="btn btn-danger" >
                    <i class="glyphicon glyphicon-remove"></i> Del
                </button>
            </div>
                <table id="server" class="table table-striped"
                data-toolbar="#toolbar-server"
                data-search="true"
                data-show-refresh="true"
                data-show-export="true"
                data-pagination="true"
                data-page-list="[10, 25, 50, 100, ALL]"
                data-filter-control="true"
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
<!--<div class="col-md-9">
    <div class="panel panel-default">
        <div class="panel-heading">公告内容</div>
        <div class="panel-body"> 
            <div id="toolbar">
                <button id="remove" class="btn btn-danger" disabled>
                    <i class="glyphicon glyphicon-remove"></i> Delete
                </button>
                <button id="btn-modify" class="btn btn-info" disabled> 
                    <i class="glyphicon glyphicon-wrench"></i> Modify 
                </button>
            </div>
            <table id="bb" class="table table-striped"
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
</div>-->
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

<div class="modal fade" id="modal-add">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                add
            </div>
            <div class="modal-body">
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">id:</span>
                    <input type="text" class="form-control" id="modal-add-id" placeholder="modal-add-id" name="modal-add-id">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">名字:</span>
                    <input type="text" class="form-control" id="modal-add-name" placeholder="xxx-服务器" name="modal-add-name">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">地址:</span>
                    <input type="text" class="form-control" id="modal-add-ip" placeholder="0.0.0.0" name="modal-add-ip">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">游戏端口:</span>
                    <input type="text" class="form-control" id="modal-add-port" placeholder="1288" name="modal-add-port">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">接口端口:</span>
                    <input type="text" class="form-control" id="modal-add-inner_port" placeholder="1289" name="modal-add-inner_port">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">状态:</span>
                    <input type="text" class="form-control" id="modal-add-status" placeholder="1_1" name="modal-add-status">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">公告:</span>
                    <input type="text" class="form-control" id="modal-add-bulletin_board" placeholder="欢迎光临" name="modal-add-bulletin_board">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">走马灯:</span>
                    <input type="text" class="form-control" id="modal-add-revolving_door" placeholder="办证 537080702" name="modal-add-revolving_door">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">QQ群号:</span>
                    <input type="text" class="form-control" id="modal-add-qq" placeholder="537080702" name="modal-add-qq">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">邮箱:</span>
                    <input type="text" class="form-control" id="modal-add-mail" placeholder="537080702@qq.com" name="modal-add-mail">
                </div>
            </div>
            <div class="modal-footer">
                <button id="modal-btn-add" class="btn btn-danger">
                    <i class="glyphicon glyphicon-plus"></i> Add
                </button>
            </div>
        </div>
    </div>
</div>

<script>
var $table = $('#code'),
$remove = $('#remove'),
$del = $('#btn-del'),
$add    = $('#btn-add'),
id = 0,
server_list_url = 'index.php?/bb/server_list?',
server_list_col = [ 
{
    field: 'select',
        checkbox: true
},
{
    field: 'id',
    title: '服务器ID',
    sortable: true,
    editable: {mode: 'inline'},
},
{
    field: 'name',
        title: '服务器名',
        editable: {
            mode: 'inline',
    }
},
{
    field: 't',
    title: '平台',
    editable: {mode: 'inline'},
    filterControl: 'select',
},
{
    field: 'web',
    title: '域名',
    editable: {mode: 'inline'},
    filterControl: 'select',
},
{
    field: 'ip',
    title: '地址',
    editable: {mode: 'inline'},
    filterControl: 'select',
},
{
    field: 'port',
    title: '游戏端口',
    editable: {mode: 'inline'},
    //filterControl: 'select',
},
{
    field: 'inner_port',
    title: '接口端口',
    editable: {mode: 'inline'},
    //filterControl: 'select',
},
{
    field: 'path',
    title: '服务器目录',
    editable: {mode: 'inline'},
},
{
    field: 'start_time',
    title: '开服时间(前面所有项加格式正确才有效)',
    editable: {mode: 'inline'},
},
{
    field: 'bulletin_board',
        title: '公告',
        editable: {
            type: 'textarea',
                mode: 'inline',
    }
},
//{
//    field: 'revolving_door',
//    title: '走马灯',
//    editable: { type: 'textarea', mode: 'inline' },
//},
{
    field: 'share_info',
    title: '分享',
    editable: { type: 'textarea', mode: 'inline' },
},
{
    field: 'qq',
    title: 'QQ',
    editable: { mod: 'inline' },
},
{
    field: 'mail',
    title: '邮箱',
    editable: { mod: 'inline' },
},
{
    field: 'status',
        title: '状态',
        editable: {
            mode: 'inline'
                            }
                    },
],
    bb_url = 'index.php?/bb/server_list?id=',
    bb_col = [{
        field: 'bulletin_board',
            title: '公告'
}],
set_url = 'index.php?/bb/set',
set_name_url = 'index.php?/bb/set_name',
set_status_url = 'index.php?/bb/set_status',
set_id_url = 'index.php?/bb/set_id',
set_t_url = 'index.php?/bb/set_t',
set_info_url = 'index.php?/bb/set_info';

$(document).ready(function() {
    //$del.prop('disabled', true);

    $('#server').bootstrapTable({
        url: server_list_url, columns: server_list_col
    });

    $('#server').on('editable-save.bs.table', function (e, field, row, value) {
        if (field == 'id') {
            $.post(set_id_url, {'info': {id: row.id, name: row.name, t: row.t}}, function (data, status) {
                alert(status);
            });
        } else if (field == 't') {
            $.post(set_t_url, {'info': {id: row.id, name: row.name, t: row.t}}, function (data, status) {
                alert(status);
            });
        } else {
            $.post(
                set_info_url,
                {
                    'info': [row.id, row.t, field, row[field]]
                },
                function (data, status) {
                    alert(status);
                }
                );
        }
    });

    $('#server').on('click-row.bs.table', function (e, row, element) {
        id = row.id;
        //$del.prop('disabled', false);
        //$modify.prop('disabled', false);
    });

    $('#bb').bootstrapTable({
        url: bb_url + id, columns: bb_col
    });

    $del.click(function () {
        //console.log($('#server').bootstrapTable('getSelections'));
        $.map($('#server').bootstrapTable('getSelections'), function (row) {
        $.post("index.php?/bb/del_svr", 
            {
                'id': row.id,
                't': row.t,
            },
            function (data, status) {
                alert(data.info);
                if (data.ret == 0) {
                    $('#server').bootstrapTable('remove', {field:'id', values:[id]});
                    $del.prop('disabled', true);
                }      
            });
        });
    });

    /*$modify.click(function () {
        window.location.href = 'index.php?/gift_cs?opt=1&id=' + id;
    });*/

    $add.click(function () {
        window.location.href = 'index.php?/bb/add_server';
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
