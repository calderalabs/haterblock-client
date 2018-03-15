type response =
  | Error
  | Success;

type t = response => unit;
