package main
type PhoneNumber struct {
   Number string `json:"number"`
   Type int `json:"type"`
}

type Person struct {
   Name string `json:"name"`
   Id int `json:"id"`
   Email string `json:"email"`
   Phone []PhoneNumber `json:"phone"`
}

type AddressBook struct {
   Person []Person `json:"person"`
}

