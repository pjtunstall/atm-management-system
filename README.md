# ATM

1. [SET-UP](#1-set-up)
2. [AUDIT](#1-audit)

## 1. Set-Up

To compile, open a terminal, cd into `atm-management-system` and enter `make`. To run the program then, enter `./atm`.

## 2. AUDIT

`Did the application prompt a choice of updating the phone number or the country?`

For the purpose of this exercise, they've defined a phone number as an `int`. This leads to some anomalies, such as the possibility of negative phone numbers, large numbers wrapping around to negative values, and the fact that leading zeros are ignored.

`Did the application display the account information and the gain of $5.84 of interest on day 10 of every month?`

Apparently in this banking system, interest is simple rather than compound. That is, assuming no deposits or withdrawals are made after the acount is created, interest is only earned on the initial amount.

Please note that the `records.txt` file of the database stores the total amount deposited minus withdrawals. The system calculates your balance based on this value, together with the interest rate for the account type, and the date.

`Create another user named "Michel".`

According to Rule 1, `users with the same name can not exist (names must be unique)`. But the given users file already contains a user called Michel. So to answer this question, you'll have to either delete Michel from `./data/users.txt` or create a user with a new name (i.e. not Michel or Alice).
