<?php
include 'common.php';
?>
<table class="table table-striped">
    <thead>
        <tr>
            <th>Symbol</th>
            <th>Name</th>
            <th>Shares</th>
            <th>Price</th>
            <th>Total</th>
        </tr>
    </thead>
    <tbody>
    <?php
	    foreach ($portfolio as $row)	
        {   
            echo("<tr>");
            echo("<td>" . $row["symbol"] . "</td>");
            echo("<td>" . $row["name"] . "</td>");
            echo("<td>" . $row["shares"] . "</td>");
            echo("<td>$" . number_format($row["price"], 2) . "</td>");
            echo("<td>$" . number_format($row["total"], 2) . "</td>");
            echo("</tr>");
        }
    ?>
    <tr>
        <br/>
        <td colspan="4">CASH</td>
        <td>$<?=number_format($users[0]["cash"], 2)?></td>
    </tr>
    </tbody>
</table>

<script>
    $(“img”).hide();
</script>
