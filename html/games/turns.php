<p>
Hi <em><?php echo $_SERVER['PHP_AUTH_USER']; ?></em> 
</p>
<h2>Uploading and downloading turns</h2>
<pre>
   Click on the buttons to
      Upload a new turn and 
      Download your current turn
</pre>
<form action="upload.php" method="post" enctype="multipart/form-data">
    <input type="file" accept=".trn" name="fileToUpload" id="fileToUpload">
    <input type="submit" value="Upload" name="submit">
</form>
<br>
<form action="download.php" method="post" enctype="multipart/form-data">
    <input type="submit" value="Download" name="submit">
</form>
<br>
<p> <em>Automatic Hosting</em> </p>
<h2>Next turn on</h2>
<!-- <p>Holidays</p> 
<p>Host will be executed manually when all turns are in</p> -->
<p>
<pre><code><b>
    <?php  readfile("../../../phost/logs/next_date.log"); ?>
</b></code></pre>
</p>
<h2>Host State</h2>
<p>Check turns utility returns: </p>
<pre><code>
    <?php readfile($game_check); ?>
</code></pre>
<br>
</body>
</html>
