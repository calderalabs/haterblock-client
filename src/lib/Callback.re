open Belt;

type response('a, 'b) =
  | Success('a)
  | Error('b);

type t('a, 'b) = response('a, 'b) => unit;

type action('a, 'b) = t('a, 'b) => unit;

let ignoreError = (callback: 'a => unit, response: response('a, 'b)) =>
  switch (response) {
  | Success(a) => callback(a)
  | Error(_error) => ()
  };

let finally = (callback: 'a => unit, response: response('a, 'b)) =>
  switch (response) {
  | Success(_) => callback()
  | Error(_) => callback()
  };
