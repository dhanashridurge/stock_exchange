const API_URL = "";  // Use relative URLs to avoid CORS issues

async function loadUsers() {
    console.log("Fetching users...");

    try {
        const res = await fetch("/users");

        if (!res.ok) throw new Error(`HTTP error! status: ${res.status}`);

        const data = await res.json();

        console.log("Users data received:", data);

        if (!data || data.length === 0) {
            document.getElementById("data").innerText = "No users found";
        } else {
            document.getElementById("data").innerText = JSON.stringify(data, null, 2);
        }

    } catch (err) {
        console.error("Error loading users:", err);
        document.getElementById("data").innerText = "Error: " + err;
    }
}

async function submitBuyOrder() {
    const userId = parseInt(document.getElementById("buyUserId").value);
    const symbol = document.getElementById("buySymbol").value;
    const qty = parseInt(document.getElementById("buyQty").value);
    const price = parseFloat(document.getElementById("buyPrice").value);

    console.log("Submitting buy order:", { userId, symbol, qty, price });

    document.getElementById("buyStatus").innerText = "Placing order...";
    document.getElementById("buyStatus").className = "status";

    try {
        const res = await fetch("/buy", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ userId, symbol, qty, price })
        });

        const msg = await res.text();

        if (!res.ok) throw new Error(msg);

        document.getElementById("buyStatus").innerText = "✔ " + msg;
        document.getElementById("buyStatus").className = "status success";

        // ✅ WAIT for backend → THEN refresh
        await loadUsers();

    } catch (err) {
        console.error("Error placing buy order:", err);
        document.getElementById("buyStatus").innerText = "❌ " + err.message;
        document.getElementById("buyStatus").className = "status error";
    }
}

async function submitSellOrder() {
    const userId = parseInt(document.getElementById("sellUserId").value);
    const symbol = document.getElementById("sellSymbol").value;
    const qty = parseInt(document.getElementById("sellQty").value);
    const price = parseFloat(document.getElementById("sellPrice").value);

    console.log("Submitting sell order:", { userId, symbol, qty, price });

    document.getElementById("sellStatus").innerText = "Placing order...";
    document.getElementById("sellStatus").className = "status";

    try {
        const res = await fetch("/sell", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ userId, symbol, qty, price })
        });

        const msg = await res.text();

        if (!res.ok) throw new Error(msg);

        document.getElementById("sellStatus").innerText = "✔ " + msg;
        document.getElementById("sellStatus").className = "status success";

        // ✅ Proper sync
        await loadUsers();

    } catch (err) {
        console.error("Error placing sell order:", err);
        document.getElementById("sellStatus").innerText = "❌ " + err.message;
        document.getElementById("sellStatus").className = "status error";
    }
}
 