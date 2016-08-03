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

<script>
    
function init_server_list() {
    var server_list_url = 'index.php?/bb/server_list?',
    server_list_col = [ 
    {field: 'check', checkbox: true},
    {field: 'id', title: '服务器ID', sortable: true },
    {field: 'name', title: '服务器名',},
    ],
    row_id = 0;

    $('#server').bootstrapTable({ url: server_list_url, columns: server_list_col });
}

</script>
