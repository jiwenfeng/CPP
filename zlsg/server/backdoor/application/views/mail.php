<!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>邮局</title>

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
            </div>
            <table id="server" class="table table-striped"
            data-toolbar="#toolbar-server"
            data-search="true"
            data-show-refresh="true"
            data-show-export="true"
            data-pagination="true"
            data-page-list="[10, 25, 50, 100, ALL]"
            >
        </table>
    </div> 
    </div>

    <div class="panel panel-default">
        <div class="panel-heading">玩家Id</div>
        <div class="panel-body"> 
            <div class="input-group">
                <span class="span-align-login input-group-addon " id="sizing-addon1">role_id:</span>
                <input type="text" class="form-control" id="role_id" placeholder="1,2,3" name="role_id">
            </div>
    </div> 
    </div>
    
    <div class="panel panel-default">
        <div class="panel-heading">邮件</div>
        <div class="panel-body"> 
            <div class="input-group">
                <span class="span-align-login input-group-addon " id="sizing-addon1">标题:</span>
                <input type="text" class="form-control" id="title" placeholder="title" name="title">
            </div>
            <div class="input-group">
                <span class="span-align-login input-group-addon " id="sizing-addon1">内容:</span>
                <input type="text" class="form-control" id="contents" placeholder="contents" name="contents">
            </div>

            <div class="input-group">
                <span class="span-align-login input-group-addon " id="sizing-addon1">物品内容:</span>
            <input type="text" class="form-control" id="items">
            </div>

            <div id="toolbar-table">
            </div>
            <table 
            id="table" 
            class="table table-striped"
            data-toolbar="#toolbar-table"
            data-search="true"
            data-show-refresh="true"
            data-show-toggle="true"
            data-show-columns="true"
            data-show-export="true"
            data-pagination="true"
            data-filter-control="true"
            data-page-list="[50, 100, 200, ALL]" 
            >
        </table>

        <div class="row">
            <br/>
            <div class="col-md-4 col-md-offset-4">
                <button id="btn-send" class="btn btn-danger">
                    <i class="glyphicon glyphicon-send"></i> Send
                </button>
                <button id="btn-send-all" class="btn btn-danger">
                    <i class="glyphicon glyphicon-plane"></i> Send All
                </button>
            </div>
        </div>
    </div>
</div>
</div>

</div>

<?php $this->load->view('help'); ?>

</body>

<script>
window.items = new Array();
    window.operateEvents = {
        'click .like' : function (e, value, row, index) {
            var b = true;
            for (index in window.items) {
                if (window.items[index].id == row.id) {
                    window.items[index].num = row.num;
                    b = false;
                }
            };

            if (b) { 
                window.items.push(row);
            };
            
            document.getElementById("items").value = JSON.stringify(window.items);
        },

        'click .remove' : function (e, value, row, index) {
            for (index in window.items) {
                if (window.items[index].id == row.id) {
                    window.items.splice(index, 1);
                }
            };

            document.getElementById("items").value = JSON.stringify(window.items);
        }
    };

    function operateFormatter(value, row, index) {
        return [
        '<a class="like" href="javascript:void(0)" title="Like">',
        '<i class="glyphicon glyphicon-heart"></i>',
        '</a>  ',
        '<a class="remove" href="javascript:void(0)" title="Remove">',
        '<i class="glyphicon glyphicon-remove"></i>',
        '</a>'
        ].join('');
    };

    $("#items").blur(function () {
        var val = document.getElementById("items").value;
        try {
            window.items = JSON.parse(val);
        } catch (e) {
            alert("格式错误");
        }
    })

var send_all_url = 'index.php?/mail/sendAll?opt=0',
    table_url = 'index.php?/gift_cs/itemsjson?opt=0',
    table_col = [
    //{field: 'check', checkbox: true},
    {field: 'operate', title: 'Operate', events: operateEvents, formatter: operateFormatter},
    {field: 'id', title: 'ID', sortable: true},
    {field: 'name', title: '名字', sortable: true},
    {field: 'type', title: '类型', sortable: true, filterControl: 'select'},
    {field: 'num', title: '个数', sortable: true, editable: { mode: 'inline' }},
    {field: 'bind', title: '绑定 填1或0', sortable: true, editable: { mode: 'inline' }},
    ],

    server_list_url = 'index.php?/bb/server_list?',
    server_list_col = [ 
    {field: 'check', checkbox: true},
    {field: 'id', title: '服务器ID', sortable: true },
    {field: 'name', title: '服务器名',},
    ],
    row_id = 0;

function initTables() {

    $('#table').on('load-success.bs.table', function (e, data) {
        /*for (index in data) {
            data[index].num  = 0; 
            data[index].bind = 1; 
            $('#table').bootstrapTable('updateRow', {'index': index, 'row': [data[index]]});
        }*/
    });

    $('#table').on('editable-save.bs.table', function (e, field, row, value) {

    });

    $('#table').on('click-row.bs.table', function (e, row, element) {

    });

    $('#table').bootstrapTable({
     url: table_url, columns: table_col
    });

    $('#server').bootstrapTable({
        url: server_list_url, columns: server_list_col
    });
};

function get() {

}

$(document).ready(function() {
    initTables();

    $("#help").click(function () {
        $('#helpModal').modal();
        $('#helpModal').on('show.bs.modal', function() {
        });
    });

    $("#btn-send").click(function () {
        var title    = document.getElementById("title").value,
        contents = document.getElementById("contents").value,
        items    = window.items,//$('#table').bootstrapTable('getSelections'),
        server   = $('#server').bootstrapTable('getSelections'),
        role_id  = document.getElementById("role_id").value;

        if (server.length == 0) {
            alert("请选择服务器");
            return ;
        }

        if (role_id == '') {
            alert("请输入用户ID");
            return ;
        }

        var b = confirm("确定要执行");
        if (!b) { 
            return ;
        }

        $.post(
            send_all_url, 
            {
                'title': title,
                'contents': contents,
                'items': items,
                'server': server,
                'all': false,
                'role_id': role_id,
            }, 
            function (data, status) {
                alert(JSON.stringify(data));
                //console.log(data);
            });
    });

    document.getElementById("title"   ).value = "系统邮件"
    document.getElementById("contents").value = "系统邮件"

    $("#btn-send-all").click(function () {
        var title    = document.getElementById("title").value,
        contents = document.getElementById("contents").value,
        items    = window.items,//$('#table').bootstrapTable('getSelections');
        server   = $('#server').bootstrapTable('getSelections');

        if (server.length == 0) {
            alert("请选择服务器");
            return ;
        }

        var b = confirm("确定要发给所有玩家");
        if (!b) { 
            return ;
        }

        $.post(
            send_all_url, 
            {
                'title': title,
                'contents': contents,
                'items': items,
                'server': server,
                'all': true,
            }, 
            function (data, status) {
                alert(JSON.stringify(data));
                //console.log(data);
            });
    });
});

</script>

</html>
