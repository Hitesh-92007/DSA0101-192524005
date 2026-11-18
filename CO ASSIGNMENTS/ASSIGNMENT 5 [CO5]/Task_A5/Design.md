# Task A5 — Recovery, Rethrowing and RAII

The approval layer detects a claim error. The settlement layer logs useful context and uses `throw;` to rethrow the same exception. A top-level layer catches the typed error and reports a safe message to the caller. It catches `std::exception` for standard-library failures and uses `catch(...)` only at the final boundary when the program must recover from any exception.

**Algorithm 3: LOG_AND_RETHROW**

```text
BEGIN PROCESS_CLAIM(claim)
    TRY
        RETURN APPROVE(claim)
    CATCH ClaimError error
        LOG error.message
        THROW SAME_EXCEPTION
    END TRY
END

BEGIN APPLICATION_BOUNDARY(claim)
    TRY
        DISPLAY PROCESS_CLAIM(claim)
    CATCH ClaimError error
        DISPLAY "claim rejected" and error.message
    CATCH std::exception error
        DISPLAY "processing error" and error.message
    CATCH ALL
        DISPLAY "unexpected failure"
    END TRY
END
```

When `f()` throws `42`, stack unwinding first destroys `Guard g("f")`, then `Guard m("main")`, and the integer handler prints `caught 42`. Exact prediction and output: `~f ~main caught 42` and a newline. `not printed` is unreachable after the throw. `throw;` preserves the active exception's type and object; `throw e;` creates a new exception from the static type of `e` and can slice a derived exception.

RAII ties each resource to an automatic object's lifetime. `std::unique_ptr`, `std::vector`, file streams and other owning objects release their resources as scopes are exited, including during exception propagation. Manual `delete` in every catch block duplicates cleanup and can omit newly added error paths. With f distinct failure or early-return paths, each resource needs cleanup review on those f paths; adding one new path creates another possible leak.

The same `O(1)` cleanup operation occurs on success or unwind. RAII keeps ownership cleanup local; manual cleanup requires each exit and catch path to be audited and can leak when paths change.
