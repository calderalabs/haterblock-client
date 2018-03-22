open Belt;

module Comment = {
  type t = {
    id: Model.id,
    body: string,
    score: int,
    videoId: int,
    rejected: bool,
  };
  include
    JsonApi.MakeDecoder(
      {
        type nonrec t = t;
        type attributes = {body: string};
        let attributesDecoder = (json: Js.Json.t) : attributes =>
          Json.Decode.{body: json |> field("body", string)};
        let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : t =>
          switch (resource.attributes) {
          | None => {
              id: resource.id,
              body: "",
              score: 0,
              videoId: 0,
              rejected: false,
            }
          | Some(attributes) => {
              id: resource.id,
              body: attributes.body,
              score: 0,
              videoId: 0,
              rejected: false,
            }
          };
      },
    );
  include
    Model.Make(
      {
        type nonrec t = t;
        let toModel = (record: t) : Model.t => {id: record.id};
      },
    );
};

module Sentiment = {
  type t =
    | Hateful
    | Negative
    | Neutral
    | Positive;
  exception InvalidSentiment;
  let sentimentMap = score =>
    switch (score) {
    | x when x >= 5 && x <= 10 => Positive
    | x when x >= (-3) && x <= 4 => Neutral
    | x when x >= (-6) && x <= (-4) => Negative
    | x when x >= (-10) && x <= (-7) => Hateful
    | _ => raise(InvalidSentiment)
    };
  let sentiment = (comment: Comment.t) => sentimentMap(comment.score);
  let filterBySentiment = (comments: list(Comment.t), filter: t) =>
    comments |> List.keep(_, comment => sentiment(comment) == filter);
};

let fetchAll = (callback: Callback.t(list(Comment.t), unit)) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/comments",
    ~callback=
      response =>
        switch (response) {
        | Success(json) => callback(Success(json |> Comment.decodeMany))
        | Error(_error) => callback(Error())
        },
    (),
  );

let reject = (callback: Callback.t(unit, unit), comment: Comment.t) => {
  let id = comment.id;
  Api.request(
    ~method=Fetch.Put,
    ~path={j|/rejections/$id|j},
    ~callback=
      response =>
        switch (response) {
        | Success(_json) => callback(Success())
        | Error(_error) => callback(Error())
        },
    (),
  );
};

let rejectAll = (callback: Callback.t(unit, unit), ids: list(Model.id)) =>
  Api.request(
    ~method=Fetch.Post,
    ~path="/rejections",
    ~body=[Json.Encode.("ids", list(int, ids))],
    ~callback=
      response =>
        switch (response) {
        | Success(_json) => callback(Success())
        | Error(_error) => callback(Error())
        },
    (),
  );
