<!DOCTYPE html>
<html lang="zh-cn">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>礼包生成器</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>

</head>

<body>
<?php $this->load->view('nav', TRUE); ?>

<div class="panel panel-default">
<div class="panel-heading">礼包生成器</div>
<div class="panel-body">
<div class="btn-group btn-group-justified" role="group" aria-label="...">
<span class="input-group-addon">礼包类型: </span>
<div class="btn-group" role="group">
<button id="btn-type-my" type="button" class="btn btn-default btn-gift-type" data-type="0">我的邀请码</button>
</div>
<div class="btn-group" role="group">
<button id="btn-type-ad" type="button" class="btn btn-default btn-gift-type" data-type="1">媒体礼包</button>
</div>
<div class="btn-group" role="group">
<button id="btn-type-fix" type="button" class="btn btn-default btn-gift-type" data-type="2">补偿礼包</button>
</div>
</div>
<br/><br/>
<div class="row">
<div class="col-lg-6">
<div class="input-group">
<span class="input-group-addon">礼包名字:</span>
<input type="text" class="form-control" id="gift-name">
</div>
</div>
<div class="col-lg-6">
<div class="input-group">
<span class="input-group-addon">礼包数量:</span>
<input type="text" class="form-control" id="gift-num">
</div>
</div>
</div>
<br/><br/>
<div class="input-group">
<span class="input-group-addon">礼包内容:</span>
<input type="text" class="form-control" id="gift-context">
</div>
<br/><br/>
<div class="panel panel-default">
<div class="panel-heading">选择道具:</div>
<div class="panel-body">
<input type="text" class="form-control" id="gift-items">
<table id="items" class="table table-striped" 
data-search="ture"
data-show-refresh="true"
data-show-toggle="true"
data-show-columns="true"
data-show-export="true"
data-show-pagination-switch="true"
data-pagination="true"
data-page-list="[10, 25, 50, 100, ALL]"
>
</table>
</div>
</div>

<br/><br/>
<div class="panel panel-default">
<div class="panel-heading">礼包有效期:</div>
<div class="panel-body">
<div class="row">
<div class="col-lg-2">
<div class="input-group">
<span class="input-group-addon">指定日期:</span>
</div>
</div>
<div class="col-lg-5">
<div class="input-group">
<span class="input-group-addon">生效日期:</span>
<input type="text" class="gift-date form-control" id="begin_date">
</div>
</div>
<div class="col-lg-5">
<div class="input-group">
<span class="input-group-addon">失效日期:</span>
<input type="text" class="gift-date form-control" id="end_date">
</div>
</div>
</div>
<div class="row">
<div class="col-lg-2">
<span class="input-group-addon">指定天数:</span>
</div>
<div class="col-lg-5">
<input type="text" class="form-control" id="day">
</div>
</div>
</div>
</div>
<br/><br/>
<div class="panel panel-default">
<div class="panel-heading">使用类型:</div>
<div class="panel-body">
<div class="input-group">
<span class="input-group-addon">每台设备可使用个数:</span>
<input type="text" class="form-control" id="limit-dev">
</div>
<div class="input-group">
<span class="input-group-addon">每个帐号可使用个数:</span>
<input type="text" class="form-control" id="limit-acc">
</div>
<div class="input-group">
<span class="input-group-addon">每个激活码可用次数:</span>
<input type="text" class="form-control" id="limit-num">
</div>
</div>
</div>
</div>
</div>

<center><button type="button" class="btn btn-info" id="add">
    <i class="glyphicon glyphicon-plus"></i>
    生成礼包
</button></center>

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
var id = 0;

function GetRequest() {
   var url = location.search; //获取url中"?"符后的字串
   var theRequest = new Object();
   if (url.indexOf("?") != -1) {
      var str = url.substr(1);
      strs = str.split("&");
      for(var i = 0; i < strs.length; i ++) {
         theRequest[strs[i].split("=")[0]]=unescape(strs[i].split("=")[1]);
      }
   }

   return theRequest;
};

function GetGift(id) {
    $.post(
        "index.php?/gift_cs/get_gift", 
        {
            'id': id,
        },
        function (data, status) {
            if (data.ret != 0) {
            // 恢复到增加
        } else {
            SetGift(data.gifts[0]);
        }
    });
};

function SetGift(gift) {
    window.gift_type = gift.type;
    if (gift.type == 0 ) {
        document.getElementById("btn-type-my").className = "btn btn-primary btn-gift-type";
        document.getElementById("btn-type-ad").className = "btn btn-default btn-gift-type";
        document.getElementById("btn-type-fix").className = "btn btn-default btn-gift-type";
    } else if (gift.type == 1) {
        document.getElementById("btn-type-my").className = "btn btn-default btn-gift-type";
        document.getElementById("btn-type-ad").className = "btn btn-primary btn-gift-type";
        document.getElementById("btn-type-fix").className = "btn btn-default btn-gift-type";
    } else {
        document.getElementById("btn-type-my").className = "btn btn-default btn-gift-type";
        document.getElementById("btn-type-ad").className = "btn btn-default btn-gift-type";
        document.getElementById("btn-type-fix").className = "btn btn-primary btn-gift-type";
    }

    document.getElementById("gift-name"   ).value = gift.name;
    document.getElementById("gift-num"    ).value = gift.num;
    document.getElementById("gift-context").value = gift.context;
    document.getElementById("begin_date"  ).value = gift.start_time;
    document.getElementById("end_date"    ).value = gift.end_time;
    document.getElementById("day"         ).value = gift.day;
    document.getElementById("limit-dev"   ).value = gift.limit_device;
    document.getElementById("limit-acc"   ).value = gift.limit_account;
    document.getElementById("limit-num"   ).value = gift.limit_use;
    document.getElementById("gift-items"  ).value = gift.items;
    window.gift_items = JSON.parse(gift.items);
};

$(document).ready(function() {
    id = GetRequest()['id'];
    if (id && id != 0) {
        GetGift(id);
        document.getElementById("add").innerHTML = "<i class=\"glyphicon glyphicon-wrench\"></i>修改礼包";      
    }

    window.gift_type = 0;

    $(".btn-gift-type").click(function() {
        window.gift_type = $(this).data('type');
    });

    $("#help").click(function() {
        console.log('help');      

        $('#helpModal').modal();
        $('#helpModal').on('show.bs.modal', function() {
            console.log('helpModal');
        });
    });

    $("#add").click(function () {
        var name       = document.getElementById("gift-name"   ).value,
         num        = document.getElementById("gift-num"    ).value, 
         context    = document.getElementById("gift-context").value,
         begin_date = document.getElementById("begin_date"  ).value,
         end_date   = document.getElementById("end_date"    ).value,
         day        = document.getElementById("day"         ).value,
         limit_dev  = document.getElementById("limit-dev"   ).value,
         limit_acc  = document.getElementById("limit-acc"   ).value,
         limit_num  = document.getElementById("limit-num"   ).value,
         items      = JSON.stringify(window.gift_items),
         url = "index.php?/gift_cs/add";

        if (id && id != 0) {
            url = "index.php?/gift_cs/modify";
        }

        //console.log(id + " " + url);

        $.post(
            url,
            {
                'gift_type'   : window.gift_type,
                'gift_name'   : name,
                'gift_num'    : num,
                'gift_context': context,
                'begin_date'  : begin_date,
                'end_date'    : end_date,
                'day'         : day,
                'limit_dev'   : limit_dev,
                'limit_acc'   : limit_acc,
                'limit_num'   : limit_num,
                'items'       : items,
                'id'          : id,
            },
            function (data, status) {
                var info = '';
                if (data.ret != 0) {
                    info = data.info;
                } else {
                    info = "成功";
                }

                alert(info + " " + JSON.stringify(data));
            });
    });

    url = 'index.php?/gift_cs/itemsjson?id=';
    col = [{field: 'id', title: 'Id', sortable: true}, 
                {field: 'type', title: 'Type', sortable: true}, 
                {field: 'name', title: 'Name', sortable: true}, 
                {field: 'num', title: 'Num', sortable: true, editable: true, align: 'center'},
                {field: 'operate', title: 'Operate', events: operateEvents, formatter: operateFormatter}];

    $('#items').bootstrapTable({
        url: url + id, columns: col
    });

    $(".gift-date").datepicker();

    $("#gift-items").blur(function () {
        var val = document.getElementById("gift-items").value;
        try {
            window.gift_items = JSON.parse(val);
        } catch (e) {
            alert("格式错误");
        }
    })
});

window.gift_items = {};
window.operateEvents = {
    'click .like' : function (e, value, row, index) {
        window.gift_items[row.id] = row.num;
        document.getElementById("gift-items").value = JSON.stringify(window.gift_items);
        //alert(JSON.stringify(row));
    },

        'click .remove' : function (e, value, row, index) {
            window.gift_items[row.id] = undefined;
            document.getElementById("gift-items").value = JSON.stringify(window.gift_items);
            //alert(JSON.stringify(row));
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

function responseHandler(res) {
    return res;
};

</script>

    </html>
