const API_URL = "";  // Use relative URLs to avoid CORS issues

function loadUsers() {
    console.log("Fetching users...");
    fetch("/users")
        .then(res => {
            console.log("Response status:", res.status);
            if (!res.ok) throw new Error(`HTTP error! status: ${res.status}`);
            return res.json();
        })
        .then(data => {
            console.log("Users data received:", data);
            if (!data || data.length === 0) {
                document.getElementById("data").innerText = "No users found";
            } else {
                document.getElementById("data").innerText = JSON.stringify(data, null, 2);
            }
        })
        .catch(err => {
            console.error("Error loading users:", err);
            document.getElementById("data").innerText = "Error: " + err.message;
        });
}

function submitBuyOrder() {
    const userId = parseInt(document.getElementById("buyUserId").value);
    const symbol = document.getElementById("buySymbol").value;
    const qty = parseInt(document.getElementById("buyQty").value);
    const price = parseFloat(document.getElementById("buyPrice").value);
    
    console.log("Submitting buy order:", { userId, symbol, qty, price });
    
    document.getElementById("buyStatus").innerText = "Placing order...";
    document.getElementById("buyStatus").className = "status";
    
    fetch("/buy", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ userId, symbol, qty, price })
    })
        .then(res => {
            if (!res.ok) throw new Error(`HTTP error! status: ${res.status}`);
            return res.text();
        })
        .then(msg => {
            console.log("Buy order response:", msg);
            document.getElementById("buyStatus").innerText = "✓ " + msg;
            document.getElementById("buyStatus").className = "status success";
            
            // Refresh users to show updated portfolio
            setTimeout(loadUsers, 500);
        })
        .catch(err => {
            console.error("Error placing buy order:", err);
            document.getElementById("buyStatus").innerText = "✗ Error: " + err.message;
            document.getElementById("buyStatus").className = "status error";
        });
}

function submitSellOrder() {
    const userId = parseInt(document.getElementById("sellUserId").value);
    const symbol = document.getElementById("sellSymbol").value;
    const qty = parseInt(document.getElementById("sellQty").value);
    const price = parseFloat(document.getElementById("sellPrice").value);
    
    console.log("Submitting sell order:", { userId, symbol, qty, price });
    
    document.getElementById("sellStatus").innerText = "Placing order...";
    document.getElementById("sellStatus").className = "status";
    
    fetch("/sell", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ userId, symbol, qty, price })
    })
        .then(res => {
            if (!res.ok) throw new Error(`HTTP error! status: ${res.status}`);
            return res.text();
        })
        .then(msg => {
            console.log("Sell order response:", msg);
            document.getElementById("sellStatus").innerText = "✓ " + msg;
            document.getElementById("sellStatus").className = "status success";
            
            // Refresh users to show updated portfolio
            setTimeout(loadUsers, 500);
        })
        .catch(err => {
            console.error("Error placing sell order:", err);
            document.getElementById("sellStatus").innerText = "✗ Error: " + err.message;
            document.getElementById("sellStatus").className = "status error";
        });
}
 