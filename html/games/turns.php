<p>
Hola <em><?php echo $_SERVER['PHP_AUTH_USER']; ?></em> 
</p>
<h2>Subir y bajar turnos</h2>
<pre>
   Pincha en el botón: 
      Upload para subir un nuevo turno y 
      Download para bajar tu turno actual.
</pre>
<form action="upload.php" method="post" enctype="multipart/form-data">
    <input type="file" accept=".trn" name="fileToUpload" id="fileToUpload">
    <input type="submit" value="Upload" name="submit">
</form>
<br>
<form action="do_download.php" method="post" enctype="multipart/form-data">
    <input type="submit" value="Download" name="submit">
</form>
<br>
<p> <em>Ejecución Automática Activada</em> </p>
<h2>Próxima ejecución será en</h2>
<!-- <p>PERIODO ESTIVAL --- No hacer caso de la fecha.</p> 
<p>La ejecución será manual cuando estén todos los turnos</p> -->
<p>
<pre><code><b>
    <?php  readfile("/home/vgap/phost/logs/next_date.log"); ?>
</b></code></pre>
</p>
<h2>Estado de los turnos</h2>
<p>La comprobacion de turnos ahora mismo muestra: </p>
<pre><code>
    <?php readfile($game_check); ?>
</code></pre>
<br>
</body>
</html>
