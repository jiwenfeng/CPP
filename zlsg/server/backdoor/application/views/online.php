<!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>在线玩家管理</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>

</head>

<body>

    <?php $this->load->view('nav', TRUE); ?>

    <div class="row">
        <div class="col-md-3">
            <?php $this->load->view('server_list'); ?>
        </div>
        <div class="col-md-9">
            <button id="btn-get" class="btn btn-info"> <i class="glyphicon glyphicon-plus-sign"></i> Get </button>

            <form class="form-inline" onsubmit="return false">
                <div class="form-group">
                <label>开始时间</label>
                <input type="text" class="form-control text-date" id="date-start" placeholder="" name="start">
                </div>
                <div class="form-group">
                <label>结束时间</label>
                <input type="text" class="form-control text-date" id="date-end" placeholder="" name="end">
                </div>
                <button id="btn-refresh" class="btn btn-info"> <i class="glyphicon glyphicon-plus-sign"></i> 刷新图表 </button>
            </form>
            <canvas id="onlineChart" width="400" height="400"></canvas>

        </div>
    </div>

    <?php $this->load->view('help'); ?>

    <script>

    $("#btn-get").click(function () {
        $server = $('#server').bootstrapTable('getSelections');
        $.post(
            'index.php?/online/get_online_player_list?opt=0',
            {
                'server': $server, 
            },
            function (data, status) { 
                alert(JSON.stringify(data)); 
            });
    });

    $("#btn-refresh").click(function () {
        var start = document.getElementById('date-start').value,
            end   = document.getElementById('date-end'  ).value;

        $.post(
            'index.php?/online/get_online?opt=0',
            {
                'start': Date.parse(start).getTime() / 1000,
                'end'  : Date.parse(end).getTime() / 1000, 
            },
            function (rsp, status) { 
                //alert(JSON.stringify(data)); 
                $('#onlineChart').replaceWith('<canvas id="onlineChart" width="400" height="400"></canvas>');
                var labels = new Array(),
                date = new Array(),
                allcount = new Array(),
                cashcount = new Array(),
                data1 = new Array(),
                data2 = new Array();
                for (index in rsp.data) {
                    d = new Date(rsp.data[index].createDate * 1000);
                    s = d.getFullYear() + '-' + (d.getMonth() + 1) + '-' + d.getDate();
                    if (date[s] == undefined) {
                        date[s] = 1;
                        allcount[s]  = rsp.data[index].allcount;
                        cashcount[s] = rsp.data[index].cashcount;
                        labels.push(s);
                    } else {
                        if (allcount[s] < rsp.data[index].allcount) {
                            allcount[s] = rsp.data[index].allcount;
                        }

                        if (cashcount[s] < rsp.data[index].cashcount) {
                            cashcount[s] < rsp.data[index].cashcount;
                        }
                    }
                };

                for (index in allcount) {
                    data1.push(allcount[index]);
                    data2.push(cashcount[index]);
                }

                console.log(data1.join());
                console.log(data2.join());

                var data = {
                    labels : labels,
                    datasets : [
                    {fillColor : "rgba(220,220,220,0.5)",
                    strokeColor : "rgba(220,220,220,1)",
                    pointColor : "rgba(220,220,220,1)",
                    pointStrokeColor : "#fff",
                    data : data1},
                    {fillColor : "rgba(151,187,205,0.5)",
                    strokeColor : "rgba(151,187,205,1)",
                    pointColor : "rgba(151,187,205,1)",
                    pointStrokeColor : "#fff",
                    data : data2}
                    ]
                }

                var ctx = $("#onlineChart").get(0).getContext("2d");
                new Chart(ctx).Line(data);
            });
    }); 

    $(document).ready(function() {
        init_server_list();
        $(".text-date").datetimepicker();
        var ctx = $("#onlineChart").get(0).getContext("2d");

        var data = {
                    labels : ['在上面输入时间查询'],
                    datasets : [
                    {fillColor : "rgba(220,220,220,0.5)",
                    strokeColor : "rgba(220,220,220,1)",
                    pointColor : "rgba(220,220,220,1)",
                    pointStrokeColor : "#fff",
                    data : [0]},
                    {fillColor : "rgba(151,187,205,0.5)",
                    strokeColor : "rgba(151,187,205,1)",
                    pointColor : "rgba(151,187,205,1)",
                    pointStrokeColor : "#fff",
                    data : [0]}
                    ]
                }
        new Chart(ctx).Line(data);
    });

    </script>

    </html>
