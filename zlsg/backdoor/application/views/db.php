<!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>数据库</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>

</head>

<body>
<?php $this->load->view('nav', TRUE); ?>

<div class="row">

  <div class="col-md-3">
    <div class="panel panel-default">
        <div class="panel-heading">表</div>
        <div class="panel-body"> 
            <div id="toolbar-tables">
                <button id="btn-add" class="btn btn-primary" disabled>
                    <i class="glyphicon glyphicon-plus"></i> Add
                </button>
            </div>
            <table id="tables" class="table table-striped"
            data-toolbar="#toolbar-tables"
            data-search="true"
            data-show-refresh="true"
            data-show-export="true"
            data-pagination="true"
            data-page-list="[10, 25, 50, 100, ALL]">
        </table>
    </div> 
</div>
</div>

<div class="col-md-9">
    <div class="panel panel-default">
        <div class="panel-heading">字段</div>
        <div class="panel-body"> 
            <div id="toolbar-fields">
                <button id="remove" class="btn btn-danger" disabled>
                    <i class="glyphicon glyphicon-remove"></i> Delete
                </button>
                <button id="btn-modify" class="btn btn-info" disabled> 
                    <i class="glyphicon glyphicon-wrench"></i> Modify 
                </button>
            </div>
            <table id="fields" class="table table-striped"
            data-toolbar="#toolbar-fields"
            data-search="true"
            data-show-refresh="true"
            data-show-toggle="true"
            data-show-columns="true"
            data-show-export="true"
            data-pagination="true"
            data-page-list="[10, 25, 50, 100, ALL]"
            >
        </table>
    </div>
</div>
</div>

</div>

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

var    
data_url = 'index.php?/db/data?name=',
fields_url = 'index.php?/db/fields?name=',
fields_col = [];
tables_url = 'index.php?/db/tables?opt=0',
   tables_col = [{field: 'name', title: '表名'}];

function initTables() {
    $('#tables').bootstrapTable({
        url: tables_url, columns: tables_col
    });

    $('#tables').on('click-row.bs.table', function (e, row, element) {
        name = row.name;
        fields_col = [];
        $('#fields').bootstrapTable('destroy');

        $.getJSON(fields_url + name, function(result) {
            for (index in result) {
                fields_col.push({field: result[index].Field, title: result[index].Field});
            }

            $('#fields').bootstrapTable({
                url: data_url + name, columns: fields_col
            });

            //$('#fields').bootstrapTable('refresh', [{columns: fields_col}]);

            /*$.getJSON(data_url + name, function(result) {
                $('#fields').bootstrapTable('columns', fields_col);
                $('#fields').bootstrapTable('load', result);
            });*/
        });

        //$remove.prop('disabled', false);
        //$modify.prop('disabled', false);
    });
};

function loadField(table_name) {

}

$(document).ready(function() {
    initTables();

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
