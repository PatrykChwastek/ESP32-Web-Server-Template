import { initializeWebSocket, registerMessageHandler } from "./socket.js";

const socket = initializeWebSocket(`ws://${window.location.hostname}/ws`);

registerMessageHandler("config", (config) => {
  const label = document.getElementById("testVal");
  label.innerText = config.test;
});

registerMessageHandler("ledStatus", ({status}) => {
  const led = document.getElementById("ledStatus");
  led.style.backgroundColor = status ? "white" : "blue";
});

const saveBtn = document.getElementById("saveBtn");
saveBtn.addEventListener("click", () => {
  const testVal = document.getElementById("test").value;
  const json = {type:'config', test: testVal };
  socket.send(JSON.stringify(json));
});
