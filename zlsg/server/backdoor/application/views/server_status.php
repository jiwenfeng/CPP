<!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>服务器状态</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>

</head>

<body>
<?php $this->load->view('nav', TRUE); ?>

<div class="row">

<div class="col-md-12">
    <div class="panel panel-default">
        <div class="panel-heading">状态信息</div>
        <div class="panel-body"> 
            <div id="toolbar-table">
                <button id="btn-start" class="btn btn-danger" disabled>
                    <i class="glyphicon glyphicon-play-circle"></i> Start
                </button>
                <button id="btn-restart" class="btn btn-danger" disabled>
                    <i class="glyphicon glyphicon-repeat"></i> Restart
                </button>
                <button id="btn-close" class="btn btn-danger" disabled> 
                    <i class="glyphicon glyphicon-off"></i> Close 
                </button>
                <button id="btn-get" class="btn btn-info" disabled> 
                    <i class="glyphicon glyphicon-user"></i> Get status and user
                </button>
            </div>
            <table id="table" class="table table-striped"
            data-toolbar="#toolbar-table"
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

</body>

<script>

var start_url = 'index.php?/server/on?opt=0',
    stop_url = 'index.php?/server/off?opt=0',
    get_url = 'index.php?/server/get?opt=0',
    set_url = 'index.php?/bb/set_info';
    table_url = 'index.php?/server/server_list?opt=0',
    table_col = [
    {field: 'check', checkbox: true},
    {field: 'id', title: 'ID', sortable: true},
    {field: 't', title: '平台', sortable: true},
    {field: 'name', title: '名字', sortable: true},
    {field: 'path', title: '目录', editable: { mode: 'inline' }},
    {field: 'status', title: '状态', sortable: true, filterControl: 'select'},
    {field: 'user', title: '玩家', sortable: true},
    ],

row_id = 0;

function initTables() {

    $('#table').on('load-success.bs.table', function (e, data) {
        for (index in data) {
            data[index].status = "点击更新";
            data[index].user   = 0; 
            $('#table').bootstrapTable('updateRow', {'index': index, 'row': [data[index]]});
        }
    });

    $('#table').on('editable-save.bs.table', function (e, field, row, value) {
        $.post(
            set_url,
            {
                'info': [row.id, row.t, field, row[field]]
            },
            function (data, status) {
                alert(status);
            }
            );
    });

    $('#table').on('click-row.bs.table', function (e, row, element) {
        row_id = row.id;
        row_t  = row.t;
        if (row.status == "点击更新") {
            row.status = '读取中...';
            get();
        }

        $("#btn-start"  ).prop('disabled', false);
        $("#btn-restart").prop('disabled', false);
        $("#btn-close"  ).prop('disabled', false);
        $("#btn-get"    ).prop('disabled', false);
    });

    $('#table').bootstrapTable({
        url: table_url, columns: table_col
    });
};

function get() {
    $.post(get_url, {'id': row_id}, function (rsp, status) {
        if (rsp.ret != 0) {
            return ;
        }

        var data = $('#table').bootstrapTable('getData');
        for (index in data) {
            if (data[index].id != row_id || data[index].t != row_t) {
                continue
            }

            data[index].status = rsp.data.status;
            data[index].user   = rsp.data.user; 
            $('#table').bootstrapTable('updateRow', {'index': index, 'row': [data[index]]});
            break;
        }
    });
}

$(document).ready(function() {
    initTables();

    $("#help").click(function () {
        $('#helpModal').modal();
        $('#helpModal').on('show.bs.modal', function() {
        });
    });

    $("#btn-start").click(function () {
        $.post(start_url, {'id': row_id}, function () {
            setTimeout("get()", 10000);
        });

        $("#btn-start"  ).prop('disabled', true);
        $("#btn-restart").prop('disabled', false);
        $("#btn-close"  ).prop('disabled', false);
    });

    $("#btn-restart").click(function () {
        alert('先按Close 等一下再按 Start');

        $("#btn-start"  ).prop('disabled', true);
        $("#btn-restart").prop('disabled', false);
        $("#btn-close"  ).prop('disabled', false);
    });

    $("#btn-close").click(function () {
        $.post(stop_url, {'id': row_id}, function () {
            setTimeout("get()", 10000);
        });

        $("#btn-start"  ).prop('disabled', false);
        $("#btn-restart").prop('disabled', false);
        $("#btn-close"  ).prop('disabled', true);
    });

    $("#btn-get").click(get);
});

</script>

</html>
