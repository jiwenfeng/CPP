<!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>客户端版本信息</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>

</head>

<body>
<?php $this->load->view('nav', TRUE); ?>

<div class="row">

<div class="col-md-12">
    <div class="panel panel-default">
        <div class="panel-heading">平台CDN列表</div>
        <div class="panel-body"> 
            <div id="toolbar-cdn">
                <button id="cdn-btn-add" class="btn btn-info">
                    <i class="glyphicon glyphicon-plus"></i> Add
                </button>
                <button id="cdn-btn-del" class="btn btn-info" >
                    <i class="glyphicon glyphicon-remove"></i> Delete
                </button>
                <!--<button id="cdn-btn-modify" class="btn btn-info" disabled> 
                    <i class="glyphicon glyphicon-wrench"></i> Modify 
                </button>-->
            </div>
            <table id="cdn" class="table table-striped"
            data-toolbar="#toolbar-cdn"
            data-search="true"
            data-show-refresh="true"
            data-show-toggle="true"
            data-show-columns="true"
            data-show-export="true"
            data-pagination="true"
            data-filter-control="true"
            data-page-list="[10, 25, 50, 100, ALL]"
            >
        </table>
    </div>
</div>

<div class="panel panel-default">
        <div class="panel-heading">客户端版本号</div>
        <div class="panel-body"> 
            <div id="toolbar-version">
                <button id="version-btn-add" class="btn btn-info">
                    <i class="glyphicon glyphicon-plus"></i> Add
                </button>
                <button id="version-btn-del" class="btn btn-info" >
                    <i class="glyphicon glyphicon-remove"></i> Delete
                </button>
            </div>
            <table id="version" class="table table-striped"
            data-toolbar="#toolbar-version"
            data-search="true"
            data-show-refresh="true"
            data-show-toggle="true"
            data-show-columns="true"
            data-show-export="true"
            data-pagination="true"
            data-filter-control="true"
            data-page-list="[10, 25, 50, 100, ALL]"
            >
        </table>
    </div>
</div>

    <div class="panel panel-default">
        <div class="panel-heading">最新文件信息列表</div>
        <div class="panel-body"> 
            <div id="new-toolbar-files">
                <button id="new-btn-add" class="btn btn-info">
                    <i class="glyphicon glyphicon-plus"></i> Add
                </button>
                <button id="new-btn-remove" class="btn btn-info" disabled>
                    <i class="glyphicon glyphicon-remove"></i> Delete
                </button>
                <button id="new-btn-modify" class="btn btn-info" disabled> 
                    <i class="glyphicon glyphicon-wrench"></i> Modify 
                </button>
            </div>
            <table id="new-files" class="table table-striped"
            data-toolbar="#new-toolbar-files"
            data-search="true"
            data-show-refresh="true"
            data-show-toggle="true"
            data-show-columns="true"
            data-show-export="true"
            data-pagination="true"
            data-filter-control="true"
            data-page-list="[10, 25, 50, 100, ALL]"
            >
        </table>
        <center><!--<button id="files-check" class="btn btn-info" >  <i class="glyphicon glyphicon-wrench"></i> 检查错误 </button>-->
        <button id="files-sync" class="btn btn-danger" >  <i class="glyphicon glyphicon-plus"></i> 同步到现网 </button></center>
    </div>
</div>
</div>

</div>

<div class="row">

<div class="col-md-12">
    <div class="panel panel-default">
        <div class="panel-heading">现网文件信息列表</div>
        <div class="panel-body"> 
            <div id="toolbar-files">
                <button id="btn-add" class="btn btn-danger">
                    <i class="glyphicon glyphicon-plus"></i> Add
                </button>
                <button id="btn-remove" class="btn btn-danger" disabled>
                    <i class="glyphicon glyphicon-remove"></i> Delete
                </button>
                <button id="btn-modify" class="btn btn-danger" disabled> 
                    <i class="glyphicon glyphicon-wrench"></i> Modify 
                </button>
            </div>
            <table id="files" class="table table-striped"
            data-toolbar="#toolbar-files"
            data-search="true"
            data-show-refresh="true"
            data-show-toggle="true"
            data-show-columns="true"
            data-show-export="true"
            data-pagination="true"
            data-filter-control="true"
            data-page-list="[10, 25, 50, 100, ALL]"
            >
        </table>
    </div>
</div>
</div>

</div>


<?php $this->load->view('help'); ?>

<div class="modal fade" id="modal-add">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                add
            </div>
            <div class="modal-body">
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">id:</span>
                    <input type="text" class="form-control" id="id" placeholder="id" name="id">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">pf:</span>
                    <input type="text" class="form-control" id="pf" placeholder="pf" name="pf">
                </div>
                <div class="input-group">
                    <div class="input-group-btn">
                        <button type="button" class="btn btn-default dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">sys:<span class="caret"></span></button>
                        <ul class="dropdown-menu">
                            <li><a href="#" class="dropdown-item-sys" id="dropdown-android">android</a></li>
                            <li><a href="#" class="dropdown-item-sys" id="dropdown-ios">ios</a></li>
                            <li><a href="#" class="dropdown-item-sys" id="dropdown-windows">windows</a></li>
                        </ul>
                    </div>
                    <input type="text" class="form-control" id="sys" placeholder="sys" name="sys">
                </div>
                <div class="input-group">
                    <div class="input-group-btn">
                        <button type="button" class="btn btn-default dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">type:<span class="caret"></span></button>
                        <ul class="dropdown-menu">
                            <li><a href="#" class="dropdown-item-type" id="dropdown-lib">lib</a></li>
                            <li><a href="#" class="dropdown-item-type" id="dropdown-res">res</a></li>
                            <li><a href="#" class="dropdown-item-type" id="dropdown-apk">apk</a></li>
                        </ul>
                    </div>
                    <input type="text" class="form-control" id="type" placeholder="type" name="type">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">url:</span>
                    <input type="text" class="form-control" id="url" placeholder="url" name="url">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">version:</span>
                    <input type="text" class="form-control" id="new-version" placeholder="version" name="version">
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

<div class="modal fade" id="cdn-modal-add">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                add
            </div>
            <div class="modal-body">
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">pf:</span>
                    <input type="text" class="form-control" id="cdn-pf" placeholder="pf" name="cdn-pf">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">cdn:</span>
                    <input type="text" class="form-control" id="cdn-url" placeholder="cdn" name="cdn-url">
                </div>
            </div>
            <div class="modal-footer">
                <button id="modal-cdn-btn-add" class="btn btn-danger">
                    <i class="glyphicon glyphicon-plus"></i> Add
                </button>
            </div>
        </div>
    </div>
</div>

  <?php 
  $params = array(
    'id' => 'modal',
    'header' => 'add',
    'body' => '
    <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">id:</span>
                    <input type="text" class="form-control" id="version-id" placeholder="id" name="id">
                </div>
    <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">type:</span>
                    <input type="text" class="form-control" id="version-type" placeholder="type" name="type">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">version:</span>
                    <input type="text" class="form-control" id="version-v" placeholder="version" name="version">
                </div>
    ',
    'footer' => '
    <button id="modal-version-btn-add" class="btn btn-danger">
                    <i class="glyphicon glyphicon-plus"></i> Add
                </button>
    ',
    );
  $this->load->view('modal', $params); 
  ?>


</body>

<script>

var    
delete_url       = 'index.php?/filesmgr/delete?opt=0',
insert_url       = 'index.php?/filesmgr/insert?opt=0',
modify_url       = 'index.php?/filesmgr/modify?opt=0',
new_delete_url   = 'index.php?/filesmgr/delete?opt=1',
new_insert_url   = 'index.php?/filesmgr/insert?opt=1',
new_modify_url   = 'index.php?/filesmgr/modify?opt=1',
set_url          = 'index.php?/filesmgr/set_files_info?opt=0',
new_set_url      = 'index.php?/filesmgr/set_files_info?opt=1',
data_url         = 'index.php?/filesmgr/files_info?opt=0',
new_files_url    = 'index.php?/filesmgr/files_info?opt=1',
files_check_url  = 'index.php?/filesmgr/files_check?opt=0',
files_update_url = 'index.php?/filesmgr/files_update?opt=0',
cdn_url          = 'index.php?/filesmgr/get_pf_cdn?opt=0',
cdn_set_url      = 'index.php?/filesmgr/set_pf_cdn?opt=0',
cdn_del_url      = 'index.php?/filesmgr/del_pf_cdn?opt=0',
version_url      = 'index.php?/filesmgr/get_version?opt=0',
version_set_url  = 'index.php?/filesmgr/set_version?opt=0',
version_del_url  = 'index.php?/filesmgr/del_version?opt=0',
tables_col = [
{field: 'id', title: 'ID', sortable: true},
{field: 'pf', title: '平台', filterControl: 'select'},
{field: 'sys', title: '系统', filterControl: 'select'},
{field: 'type', title: '类型', filterControl: 'select'},
{field: 'url', title: '地址', editable: {
    mode: 'inline',
}},
{field: 'version', title: '版本', filterControl: 'select', sortable: true, editable: {
    mode: 'inline',
}},
],
cdn_tables_col = [
{field: 'state', checkbox: true},
{field: 'pf', title: '平台', filterControl: 'select'},
{field: 'cdn', title: '地址', editable: { mode: 'inline'}},
],
version_tables_col = [
{field: 'state', checkbox: true},
{field: 'id', title: 'id', filterControl: 'select'},
{field: 'type', title: '类型', editable: { mode: 'inline'}},
{field: 'version', title: '版本', editable: { mode: 'inline'}},
]
row_id = 0;

function save_field(url, field, row) {
    var args = {set: []};
    var item = {id: row.id};
    item[field] = row[field];
    args.set.push(item);
    $.post(
        url,
        args,//{set: [{id: row.id, field: row[field]}]},
        function (data, status) {
            alert(JSON.stringify(data));
        });
};

function initTables() {
    $('#cdn').bootstrapTable({
        url: cdn_url, columns: cdn_tables_col
    });

    $('#version').bootstrapTable({
        url: version_url, columns: version_tables_col
    });

    $('#files').bootstrapTable({
        url: data_url, columns: tables_col
    });

    $('#new-files').bootstrapTable({
        url: new_files_url, columns: tables_col
    });

    $('#cdn').on('editable-save.bs.table', function (e, field, row, value) {
        $.post(cdn_set_url, {'row': row}, function (data, status) { alert(JSON.stringify(data)); });
    });

    $('#version').on('editable-save.bs.table', function (e, field, row, value) {
        $.post(version_set_url, {'row': row}, function (data, status) { alert(JSON.stringify(data)); });
    });

    $('#files').on('editable-save.bs.table', function (e, field, row, value) {
        save_field(set_url, field, row);
    });

    $('#new-files').on('editable-save.bs.table', function (e, field, row, value) {
        save_field(new_set_url, field, row);
    });

    $('#files').on('click-row.bs.table', function (e, row, element) {
        row_id = row.id;
        disable_btn();
        $("#btn-remove").prop('disabled', false);
        $("#btn-modify").prop('disabled', false);
        //$modify.prop('disabled', false);
    });

    $('#new-files').on('click-row.bs.table', function (e, row, element) {
        row_id = row.id;
        disable_btn();
        $("#new-btn-remove").prop('disabled', false);
        $("#new-btn-modify").prop('disabled', false);
        //$modify.prop('disabled', false);
    });
};

function disable_btn() {
    $("#btn-remove").prop('disabled', true);
    $("#btn-modify").prop('disabled', true);
    $("#new-btn-remove").prop('disabled', true);
    $("#new-btn-modify").prop('disabled', true);
};

function post_insert() {
        $('#modal-add').on('show.bs.modal', function() {
        document.getElementById("id").value       = "0";   
        document.getElementById("pf").value       = "qq";
        document.getElementById("sys").value      = "";
        document.getElementById("type").value     = "";
        document.getElementById("url").value      = "";
        document.getElementById("new-version").value  = "";
        document.getElementById("modal-btn-add").innerHTML = "<i class=\"glyphicon glyphicon-plus\"></i>add"
        
        opt = 'add';
        });

        /*$('#modal-add').on('show.bs.modal', function() {
        });*/

        $('#modal-add').modal();
};

function post_remove(url, table_id) {
    $.post(
        url,
        {
            'id': row_id,
        },
        function (data, status) {
            alert(JSON.stringify(data));
            $(table_id).bootstrapTable('remove', {field:'id', values:[row_id]});
        });

    disable_btn();
};

function post_modify(table_id) {
    $('#modal-add').on('show.bs.modal', function() {
        var data = $(table_id).bootstrapTable('getData');
            for (index in data) {
                if (data[index].id == row_id) {
                    opt = 'modify';
                    var row = data[index];
                    //console.log(row);
                    document.getElementById("id").value      = row.id;
                    document.getElementById("pf").value      = row.pf;
                    document.getElementById("sys").value     = row.sys;
                    document.getElementById("type").value    = row.type;
                    document.getElementById("url").value     = row.url;
                    document.getElementById("new-version").value = row.version;

                    document.getElementById("modal-btn-add").innerHTML = "<i class=\"glyphicon glyphicon-wrench\"></i>modify"
                    break;
                }
            }
        });

    $('#modal-add').modal();

    disable_btn();
};

$(document).ready(function() {
    initTables();

    $("#help").click(function () {
        $('#helpModal').modal();
        $('#helpModal').on('show.bs.modal', function() {
        });
    });

    $("#btn-add").click(function () {
        update = false;
        post_insert();
    });

    $("#new-btn-add").click(function () {
        update = true;
        post_insert();
    });

    $("#modal-btn-add").click(function () {
        var id      = document.getElementById("id").value;
        var pf      = document.getElementById("pf").value;
        var sys     = document.getElementById("sys").value;
        var type    = document.getElementById("type").value;
        var url     = document.getElementById("url").value;
        var version = document.getElementById("new-version").value;

        var obj = {
                'id': id,
                'pf': pf,
                'sys': sys,
                'type': type,
                'url': url,
                'version': version
            };

            var url = "";
            if (opt === "add") {
                if (update) {
                    url = new_insert_url;
                } else {
                    url = insert_url;
                }
                
            } else {
                if (update) {
                    url = new_modify_url;
                } else {
                    url = modify_url;
                }
            }

            table_id = '#files';
            if (update) {
                table_id = '#new-files';
            }

        $.post(
            url,
            obj,
            function (rsp, status) {
                var rows = [];
                obj.id = rsp.data.id;
                rows.push(obj);
                $(table_id).bootstrapTable('remove', {field:'id', values:[obj.id]});
                $(table_id).bootstrapTable('prepend', rows);
                $('#modal-add').modal('hide');

                alert(JSON.stringify(rsp));
            });
    });

    $("#btn-remove").click(function () {
        post_remove(delete_url, '#files');
    });

    $("#new-btn-remove").click(function () {
        post_remove(new_delete_url, '#new-files');
    });

    $("#btn-modify").click(function () {
        update = false;
        post_modify('#files');
    });

    $("#new-btn-modify").click(function () {
        update = true;
        post_modify('#new-files');
    });

    $(".dropdown-item-sys").click(function (e) {
        document.getElementById("sys").value     = e.toElement.innerText;
    });

    $(".dropdown-item-type").click(function (e) {
        document.getElementById("type").value     = e.toElement.innerText;
    });

    $("#files-check").click(function (e) {
        var files = $('#files').bootstrapTable('getData');
        var new_files = $('#new-files').bootstrapTable('getData');
        for (index in new_files) {
            for(fi in files) {
                if (new_files[index]['id'] == files[fi]['id']) {
                    alert('有重复ID: ' + new_files[index]['id']);
                    $("#files-sync").prop('disabled', true);
                    return ;
                }
            }
        }

        $("#files-sync").prop('disabled', false);
        /*$.post(
            files_check_url, 
            {}, 
            function (data, status) {
                alert(JSON.stringify(data));
            });*/
    });

    $("#files-sync").click(function (e) {
        /*var files = $('#files').bootstrapTable('getData');
        var new_files = $('#new-files').bootstrapTable('getData');
        for (index in new_files) {
            for(fi in files) {
                if (new_files[index]['id'] == files[fi]['id']) {
                    alert('有重复ID: ' + new_files[index]['id']);
                    return ;
                }
            }
        }*/

        $.post(
            files_update_url, 
            {}, 
            function (data, status) {
                $('#files').bootstrapTable('refresh');
                alert(JSON.stringify(data));
            });
    });

    $('#cdn-btn-add').click(function (e) {
        $('#cdn-modal-add').modal();
    });

    $('#cdn-btn-del').click(function (e) {
        rows = $('#cdn').bootstrapTable('getSelections');
        for (index in rows) {
            rows[index].state = undefined;
        }

        $.post(cdn_del_url, {'rows': rows}, function (data, status) {
            alert(JSON.stringify(data)); 
            $('#cdn').bootstrapTable('refresh');
        });
    });

    $('#modal-cdn-btn-add').click(function (e) {
        var row = {};
        row.pf  = document.getElementById('cdn-pf').value;
        row.cdn = document.getElementById('cdn-url').value;
        $.post(cdn_set_url, {'row': row}, function (data, status) { 
            alert(JSON.stringify(data)); 
            $('#cdn').bootstrapTable('refresh');
            $('#cdn-modal-add').modal('hide');
        });
    });

    $('#version-btn-add').click(function (e) {
        $('#modal').modal();
    });

    $('#version-btn-del').click(function (e) {
        rows = $('#version').bootstrapTable('getSelections');
        for (index in rows) {
            rows[index].state = undefined;
        }

        $.post(version_del_url, {'rows': rows}, function (data, status) {
            alert(JSON.stringify(data)); 
            $('#version').bootstrapTable('refresh');
        });
    });

    $('#modal-version-btn-add').click(function (e) {
        var row = {};
        row.id      = document.getElementById('version-id').value;
        row.type    = document.getElementById('version-type').value;
        row.version = document.getElementById('version-v').value;
        $.post(version_set_url, {'row': row}, function (data, status) { 
            alert(JSON.stringify(data)); 
            $('#version').bootstrapTable('refresh');
            $('#modal').modal('hide');
        });
    });
});

</script>

</html>
