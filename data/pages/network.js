const networkForm = document.getElementById("networkForm");
networkForm.addEventListener("submit", (event) => {
  event.preventDefault();

  fetch("/network", {
    method: "POST",
    body: new FormData(networkForm),
  });
});
