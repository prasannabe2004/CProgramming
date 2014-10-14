<?php
include 'common.php';
?>
<form action="quote.php" method="post">
    <fieldset>
        <div class="form-group">
            <input autofocus name="symbol" placeholder="Stock Symbol" type="text"/>
        </div>
        <div class="form-group">
            <button type="submit" class="btn">Search</button>
        </div>
    </fieldset>
</form>
