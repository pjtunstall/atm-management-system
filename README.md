# ATM

1. [SET-UP](#1-set-up)
2. [AUDIT](#1-audit)

## 1. Set-Up

To compile, open a terminal, cd into `atm-management-system` and enter `make`. To run the program then, enter `./atm`.

## 2. AUDIT

```
Did the application prompt a choice of updating the phone number or the country?
```

For the purpose of this exercise, they've defined a phone number as an `int`. This leads to some anomalies, such as the possibility of negative phone numbers, large numbers wrapping around to negative values, and the fact that leading zeros are ignored.

```
Did the application display the account information and the gain of $40.05 of interest on 10/10/2013?`
```

This is not asked for, as such, in the instructions. See 3.2. It just says to display the amount of interest per month: `For example: for an account of type savings with a deposit date of 10/10/2002 and an amount of $1023.20 the system will show "You will get $5.97 as interest on day 10 of every month".` To lazily cover these audit questions, I've also chosen to display examples three years into the future.

```
Create another user named "Michel".
```

According to Rule 1, `users with the same name can not exist (names must be unique)`. But the given users file already contains a user called Michel. So to answer this question, you'll have to either delete Michel from `./data/users.txt` or create a user with a new name (i.e. not Michel or Alice).
