<!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>白名单</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>

</head>

<body>
<?php $this->load->view('nav', TRUE); ?>

<div class="row">

<div class="col-md-12">
    <div class="panel panel-default">
        <div class="panel-heading">白名单</div>
        <div class="panel-body"> 
            <div id="toolbar-table">
                <button id="btn-add" class="btn btn-danger" >
                    <i class="glyphicon glyphicon-plus-sign"></i> add 
                </button>
                <button id="btn-del" class="btn btn-danger" >
                    <i class="glyphicon glyphicon-remove-sign"></i> delete
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

<div class="modal fade" id="modal-add">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                add
            </div>
            <div class="modal-body">
                <!--<div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">id&nbsp;:</span>
                    <input type="text" class="form-control" id="modal-add-id" placeholder="id" name="id">
                </div>-->
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">open_id:</span>
                    <input type="text" class="form-control" id="modal-add-open_id" placeholder="open_id" name="open_id">
                </div>
                <!--<div class="input-group">
                    <div class="input-group-btn">
                        <button type="button" class="btn btn-default dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">sys:<span class="caret"></span></button>
                        <ul class="dropdown-menu">
                            <li><a href="#" class="dropdown-item-sys" id="dropdown-android">0</a></li>
                            <li><a href="#" class="dropdown-item-sys" id="dropdown-ios">1</a></li>
                            <li><a href="#" class="dropdown-item-sys" id="dropdown-windows">2</a></li>
                        </ul>
                    </div>
                    <input type="text" class="form-control" id="sys" placeholder="sys" name="sys">
                </div>-->
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">group:</span>
                    <input type="text" class="form-control" id="modal-add-group" placeholder="group" name="group">
                </div>
                <div class="input-group">
                    <span class="span-align-login input-group-addon " id="sizing-addon1">remark:</span>
                    <input type="text" class="form-control" id="modal-add-remark" placeholder="remark" name="remark">
                </div>
            </div>
            <div class="modal-footer">
                <button id="modal-btn-add" class="btn btn-danger">
                    <i class="glyphicon glyphicon-plus-sign"></i> add
                </button>
            </div>
        </div>
    </div>
</body>

<script>

var table_url = 'index.php?/white_list/table?opt=0',
    get_url   = 'index.php?/white_list/get?opt=0',
    add_url   = 'index.php?/white_list/add?opt=0',
    del_url   = 'index.php?/white_list/del?opt=0';
    set_url   = 'index.php?/white_list/set?opt=0';
    table_col = [ 
    {field: 'check', checkbox: true},
    ],
    row_id = 0;

function initTables() {

    /*$('#table').on('load-success.bs.table', function (e, data) {
    });*/

    $('#table').on('editable-save.bs.table', function (e, field, row, value) {
        row.check = undefined;
        $.post(
            set_url,
            {
                row: row
            },
            function (rsp, status) {
                alert(status);
            }
            );
    });

    /*$('#table').on('click-row.bs.table', function (e, row, element) {
    });*/

    $('#table').bootstrapTable({
        url: get_url, columns: table_col
    });
};

$(document).ready(function() {
    $.post(table_url, {}, function (rsp, status) {
        for (index in rsp) {
            var col = {
                field: rsp[index].Field,
                title: rsp[index].Comment,
                sortable: true,
                filterControl: 'select',
                editable: {mode:'inline'},
                }
           table_col.push(col); 
        }
        initTables();
        });

    $("#help").click(function () {
        $('#helpModal').modal();
        $('#helpModal').on('show.bs.modal', function() {
        });
    });

    $("#btn-add").click(function () {
        $('#modal-add').modal();
    });

    $("#btn-del").click(function () {
        items    = $('#table').bootstrapTable('getSelections');
        $.post(del_url,
        {items: items},
            function (rsp, status) {
                alert(status);
                for (index in rsp.data) {
                    var obj = rsp.data[index];
                    $('#table').bootstrapTable('remove', {field:'id', values:[obj.id]});
                }
                });
    });

    $("#modal-btn-add").click(function () {
        var //id      = document.getElementById("id").value,
            open_id = document.getElementById("modal-add-open_id").value,
            group   = document.getElementById("modal-add-group"  ).value,
            remark  = document.getElementById("modal-add-remark" ).value;
        $.post(
               add_url, 
               {
                    //id      : id,
                    open_id : open_id,
                    group   : group,
                    remark  : remark,
               },
               function (rsp, status) {
                   var obj = rsp.data;
                   var rows = [obj];
                   $('#table').bootstrapTable('remove', {field:'id', values:[obj.id]});
                   $('#table').bootstrapTable('prepend', rows);
                   $('#modal-add').modal('hide');
           });
    });
});

</script>

</html>
