<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <title>Welcome to zlsg backdoor</title>

    
<?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>

    <style type="text/css">

    ::selection{ background-color: #E13300; color: white; }
    ::moz-selection{ background-color: #E13300; color: white; }
    ::webkit-selection{ background-color: #E13300; color: white; }

    body {
        background-color: #fff;
        margin: 40px;
        font: 13px/20px normal Helvetica, Arial, sans-serif;
        color: #4F5155;
    }

    a {
        color: #003399;
        background-color: transparent;
        font-weight: normal;
    }

    h1 {
        color: #444;
        background-color: transparent;
        border-bottom: 1px solid #D0D0D0;
        font-size: 19px;
        font-weight: normal;
        margin: 0 0 14px 0;
        padding: 14px 15px 10px 15px;
    }

    code {
        font-family: Consolas, Monaco, Courier New, Courier, monospace;
        font-size: 12px;
        background-color: #f9f9f9;
        border: 1px solid #D0D0D0;
        color: #002166;
        display: block;
        margin: 14px 0 14px 0;
        padding: 12px 10px 12px 10px;
    }

    #body{
        margin: 0 15px 0 15px;
    }

    p.footer{
        text-align: right;
        font-size: 11px;
        border-top: 1px solid #D0D0D0;
        line-height: 32px;
        padding: 0 10px 0 10px;
        margin: 20px 0 0 0;
    }

    #container{
        margin: 10px;
        border: 1px solid #D0D0D0;
        -webkit-box-shadow: 0 0 8px #D0D0D0;
    }
</style>

</head>
<body>
<?php $this->load->view('nav', TRUE); ?>
    <div id="container">
        <h1>Welcome to zlsg backdoor!</h1>

        <div id="body">
            <p>The page you are looking at is being generated dynamically by CodeIgniter.</p>

            <p>If you would like to edit this page you'll find it located at:</p>
            <code>application/views/server.php</code>

            <p>The corresponding controller for this page is found at:</p>
            <code>application/controllers/server.php</code>

            <p>If you are exploring CodeIgniter for the very first time, you should start by reading the <a href="user_guide/">User Guide</a>.</p>

            <p>当前服务器状态</p>
            <code><?php echo $status, " pid:", $pid; ?></code>

            <?php if ($pid == "") {
                echo "<form action=\"index.php?/server/start\" method=\"post\">
                    <input type=\"submit\" name=\"start\" value=\"启动\">
                    </form>";
            } else {
                echo "<form action=\"index.php?/server/close\" method=\"post\">
                    <input type=\"submit\" name=\"close\" value=\"关闭\">
                    </form>";
            }

            echo "<br><form action=\"index.php?/server\" method=\"post\">
                <input type=\"submit\" name=\"refresh\" value=\"刷新\">
            </form>"
             ?>

        </div>

        <p class="footer">Page rendered in <strong>{elapsed_time}</strong> seconds</p>
    </div>

</body>
</html>
