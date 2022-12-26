const contactMeForm = document.querySelector("#contact-me-form");

contactMeForm?.addEventListener("submit", (event) => {
  event.preventDefault();

  // Get the form
  const form = event.target;
  // Get the form elements
  const elements = form.elements;

  // Gather the input values
  const firstName = elements["firstName"].value;
  const lastName = elements["lastName"].value;
  const email = elements["email"].value;
  const message = elements["message"].value;

  const body = `Dear Ben,\n\n${message}\n\nKind regards,\n${firstName} ${lastName}\n\nPlease reply to: ${email}`;
  // TODO: Send an email to me with the body above
  // but we just alert the user about the intention
  alert(body);

  // Then we reset the form
  form.reset();
});
