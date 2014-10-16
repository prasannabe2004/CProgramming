<?php
include 'common.php';
?>

<form action="sell.php" method="post">
    <fieldset>
        <div class="control-group">
            <select name="symbol">
                <?php               
	                foreach ($stocks as $symbol)	
                    {   
                        echo("<option value='$symbol'>" . $symbol . "</option>");
                    }
                ?>
            </select>
        </div>
        <div class="control-group">
            <button type="submit" class="btn">Sell</button>
        </div>
    </fieldset>
</form>
