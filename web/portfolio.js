let currentUserId = null;
let refreshInterval = null;
const REFRESH_INTERVAL = 2000; // Refresh every 2 seconds

document.addEventListener("DOMContentLoaded", loadUsers);

function cls(v) {
  return v.startsWith('+') ? 'pos' : 'neg';
}

function loadUsers() {
  console.log("Fetching users...");

  fetch("/users")
    .then(res => {
      console.log("Users response status:", res.status);
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      return res.json();
    })
    .then(users => {
      console.log("Users received:", users);

      const sel = document.getElementById("userSelect");
      if (!sel) {
        console.error("userSelect element not found");
        return;
      }

      users.forEach(u => {
        const opt = document.createElement("option");
        opt.value = u.id;
        opt.textContent = `${u.name} (ID ${u.id})`;
        sel.appendChild(opt);
      });

      sel.onchange = () => {
        if (sel.value) {
          // Stop previous polling if any
          if (refreshInterval) {
            clearInterval(refreshInterval);
          }

          currentUserId = sel.value;
          loadPortfolio(sel.value);

          // Start polling for updates
          refreshInterval = setInterval(() => {
            loadPortfolio(currentUserId);
          }, REFRESH_INTERVAL);
        }
      };
    })
    .catch(err => {
      console.error("Error loading users:", err);
    });
}

function loadPortfolio(userId) {
  console.log("Loading portfolio for user:", userId);

  fetch(`/portfolio/${userId}`)
    .then(res => {
      console.log("Portfolio response status:", res.status);
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      return res.json();
    })
    .then(data => {
      const p = data[0];

      let rows = "";
      for (const [symbol, h] of Object.entries(p.portfolio)) {
        rows += `
          <tr>
            <td>${symbol}</td>
            <td>${h.quantity}</td>
            <td>${h.avg_price}</td>
            <td>${h.last_price}</td>
            <td>${h.value}</td>
            <td class="${cls(h.p_l)}">${h.p_l}</td>
            <td class="${cls(h.p_l_pct)}">${h.p_l_pct}</td>
          </tr>`;
      }

      document.getElementById("summary").innerHTML = `
        <div style="margin-bottom: 20px;">
          <button onclick="refreshPortfolio()">🔄 Refresh Now</button>
          <span style="color: #666; margin-left: 10px; font-size: 12px;">Auto-refreshing every 2 seconds...</span>
        </div>

        <h3>${p.name}</h3>

        <table>
          <tr><th>Cash</th><td>${p.cash}</td></tr>
          <tr><th>NAV</th><td>${p.nav}</td></tr>
          <tr>
            <th>Net P/L</th>
            <td class="${cls(p.net.p_l)}">
              ${p.net.p_l} (${p.net.p_l_pct})
            </td>
          </tr>
        </table>

        <h3>Holdings</h3>
        <table>
          <tr>
            <th>Symbol</th>
            <th>Qty</th>
            <th>Avg Price</th>
            <th>Last Price</th>
            <th>Value</th>
            <th>P/L</th>
            <th>P/L %</th>
          </tr>
          ${rows}
        </table>
      `;
    })
    .catch(err => {
      console.error("Error loading portfolio:", err);
    });
}

function refreshPortfolio() {
  if (currentUserId) {
    console.log("Manual refresh triggered for user:", currentUserId);
    loadPortfolio(currentUserId);
  }
}
