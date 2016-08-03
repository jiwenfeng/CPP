<!DOCTYPE html>
<html lang="zh-cn">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>钱</title>

    <?php $this->load->view('css'); ?>
    <?php $this->load->view('js'); ?>

</head>

<body>

    <?php $this->load->view('nav', TRUE); ?>
    <?php $this->load->view('help'); ?>

    <div class="row">

        <div class="col-md-3">
            <?php $this->load->view('server_list'); ?> 
        </div>

        <div class="col-md-9">
            <div class="panel panel-default">
                <div class="panel-heading" id="table-name"></div>
                <div class="panel-body"> 
                    <div id="toolbar-table">
                    </div>
                    <table id="table" class="table table-striped" data-toolbar="#toolbar-table" data-search="true" data-show-refresh="true" data-show-toggle="true" data-show-columns="true" data-show-export="true" data-pagination="true" data-filter-control="true" data-page-list="[10, 25, 50, 100, ALL]" >
                    </table>
                </div>
            </div>
        </div>

    </div>

</body>

<script>

var 
url = 'index.php?/money/get?opt=1',
table_data = new Array(),
table_col = [
{field: 'date', title: '日期'    },
{field: 'all' , title: '当日总数 单位：元'},
{field: 'account' , title: '当日充值总帐户数'},
{field: 'order' , title: '当日订单总数'},
];

function initTables() {
    //$('#table').on('editable-save.bs.table', function (e, field, row, value) {
    //});

    $('#table').bootstrapTable({
        columns: table_col
    });
};

$(document).ready(function() {
    initTables();

    $.post(url, {}, function (rsp, status) {
        var arr = JSON.parse(rsp),
         all = 0,
         ut = 0,
        account = new Array(),
        order = new Array();
        for (index in arr) {
            all += parseInt(arr[index].money);
            ut = arr[index].createDate * 1000;
            var insert = true,
            row = new Array(),
            date = new Date(ut),
            m = date.getMonth() + 1;
            row.date = date.getFullYear() + '-' + m + '-' + date.getDate();
            if (account[row.date] == undefined) {
                account[row.date] = new Array();
                account[row.date].account = 0;
                order[row.date] = new Array();
                order[row.date].order = 0;
            }

            if (account[row.date][arr[index].accountName] == undefined) {
                account[row.date][arr[index].accountName] = 1;
                account[row.date].account += 1;
            }
    
            if (order[row.date][arr[index].transactionId] == undefined) {
                order[row.date][arr[index].transactionId] = 1;
                order[row.date].order += 1;
            }

            row.all     = parseInt(arr[index].money);
            row.account = account[row.date].account;
            row.order   = order[row.date].order;
            for (i in table_data) {
                if (table_data[i].date == row.date) {
                    table_data[i].all += row.all;
                    table_data[i].account = row.account;
                    table_data[i].order   = row.order;
                    insert = false;
                }
            };

            if (insert) {
                table_data.push(row);
            }   
        }

        document.getElementById("table-name").innerHTML = "总充值" + all + "元";

        $('#table').bootstrapTable('load', table_data);
    });

    $("#help").click(function () {
        $('#helpModal').modal();
        $('#helpModal').on('show.bs.modal', function() {
        });
    });
});

</script>

</html>
