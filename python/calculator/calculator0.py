def get_int(prompt: str):
  value = input(prompt)

  try:
    num = int(value)
  except:
    print(f"'{value}' <-- is not an int")
    exit()

  return num

x = get_int("x: ")
y = get_int("y: ")

print(x + y)
