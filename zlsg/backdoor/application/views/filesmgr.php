<!DOCTYPE html>
<html lang="zh-cn">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>静态文件版本管理器</title>

  <?php $this->load->view('css'); ?>
  <?php $this->load->view('js'); ?>
  
</head>
<body>
    <?php $this->load->view('nav', TRUE); ?>

    <div class="panel panel-default">
        <div class="panel-heading">Android</div>
        <div class="panel-body">
            <form class="form-inline" action="index.php?/filesmgr/android" method="post" enctype="multipart/form-data">
                <div class="form-group">
                    <p>更新库文件 : </p>
                </div>
                <div class="form-group">
                    <input class="btn btn-default" type="file" name="lib" value="文件路径">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="exampleInputPassword3">文件版本</label>
                    <input type="text" class="form-control" name="exeversion" id="exampleInputPassword3" placeholder="文件版本">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="pf">平台</label>
                    <input type="text" class="form-control" name="pf" id="pf" placeholder="平台">
                </div>
                <button type="submit" class="btn btn-default">上传</button>
                <p>文件信息: <?php echo $android_lib_info; ?></p>
            </form>

            <form class="form-inline" action="index.php?/filesmgr/android" method="post" enctype="multipart/form-data">
                <div class="form-group">
                <p>更新资源文件 : </p>
                </div>
                <div class="form-group">
                    <input class="btn btn-default" type="file" name="res" value="文件路径">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="exampleInputPassword3">文件版本</label>
                    <input type="text" class="form-control" name="resversion" id="Text1" placeholder="文件版本">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="pf">平台</label>
                    <input type="text" class="form-control" name="pf" id="pf" placeholder="平台">
                </div>
                <button type="submit" class="btn btn-default">上传</button>
                <p>文件信息 : <?php echo $android_res_info; ?></p>
            </form>

            <form class="form-inline" action="index.php?/filesmgr/android" method="post" enctype="multipart/form-data">
                <div class="form-group">
                <p>更新游戏文件: </p><!--当前版本: <?php echo $apkversion; ?></p>-->
                </div>
                <div class="form-group">
                    <input class="btn btn-default" type="file" name="apk" value="文件路径">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="exampleInputPassword3">文件版本</label>
                    <input type="text" class="form-control" name="apkversion" id="version" placeholder="文件版本">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="pf">平台</label>
                    <input type="text" class="form-control" name="pf" id="pf" placeholder="平台">
                </div>
                <button type="submit" class="btn btn-default">上传</button>
                <p>文件信息 : <?php echo $android_app_info; ?></p>
            </form>
        </div>
    </div>

    <div class="panel panel-default">
        <div class="panel-heading">IOS</div>
        <div class="panel-body">
            <form class="form-inline" action="index.php?/filesmgr/ios" method="post" enctype="multipart/form-data">
                <div class="form-group">
                <p>执行文件 当前版本: <?php echo $ios_ipa; ?></p>
                </div>
                <div class="form-group">
                    <input class="btn btn-default" type="file" name="ipa" value="文件路径">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="exampleInputPassword3">文件版本</label>
                    <input type="text" class="form-control" id="Text3" name="ipaversion" placeholder="文件版本">
                </div>
                <button type="submit" class="btn btn-default">上传</button>
            </form>
            <form class="form-inline" action="index.php?/filesmgr/ios" method="post" enctype="multipart/form-data">
                <div class="form-group">
                <p>资源文件 当前版本: <?php echo $ios_res; ?></p>
                </div>
                <div class="form-group">
                    <input class="btn btn-default" type="file" name="res" value="文件路径">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="exampleInputPassword3">文件版本</label>
                    <input type="text" class="form-control" id="Text4" name="resversion" placeholder="文件版本">
                </div>
                <button type="submit" class="btn btn-default">上传</button>
            </form>
        </div>
    </div>

    <div class="panel panel-default">
        <div class="panel-heading">Win32</div>
        <div class="panel-body">
            <form class="form-inline" action="index.php?/filesmgr/win32?type=lib" method="post" enctype="multipart/form-data">
                <div class="form-group">
                <p>更新库文件 : </p>
                </div>
                <div class="form-group">
                    <input class="btn btn-default" type="file" name="path" value="文件路径">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="exampleInputPassword3">文件版本</label>
                    <input type="text" class="form-control" name="version" placeholder="文件版本">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="pf">平台</label>
                    <input type="text" class="form-control" name="pf" id="pf" placeholder="平台">
                </div>
                <button type="submit" class="btn btn-default">上传</button>
                <p>文件信息 : <?php echo $libinfo; ?></p>
            </form>

            <form class="form-inline" action="index.php?/filesmgr/win32?type=res" method="post" enctype="multipart/form-data">
                <div class="form-group">
                <p>更新资源文件 : </p>
                </div>
                <div class="form-group">
                    <input class="btn btn-default" type="file" name="path" value="文件路径">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="exampleInputPassword3">文件版本</label>
                    <input type="text" class="form-control" name="version" placeholder="文件版本">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="pf">平台</label>
                    <input type="text" class="form-control" name="pf" id="pf" placeholder="平台">
                </div>
                <button type="submit" class="btn btn-default">上传</button>
                <p>文件信息 : <?php echo $resinfo; ?></p>
            </form>

            <form class="form-inline" action="index.php?/filesmgr/win32?type=exe" method="post" enctype="multipart/form-data">
                <div class="form-group">
                <p>更新游戏文件 : </p>
                </div>
                <div class="form-group">
                    <input class="btn btn-default" type="file" name="path" value="文件路径">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="exampleInputPassword3">文件版本</label>
                    <input type="text" class="form-control" name="version" placeholder="文件版本">
                </div>
                <div class="form-group">
                    <label class="sr-only" for="pf">平台</label>
                    <input type="text" class="form-control" name="pf" id="pf" placeholder="平台">
                </div>
                <button type="submit" class="btn btn-default">上传</button>
                <p>文件信息 : <?php echo $exeinfo; ?></p>
            </form>
        </div>
    </div>

</body>
</html>
