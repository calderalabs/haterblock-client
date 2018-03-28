open Belt;

open MomentRe;

module Status = {
  exception InvalidStatus;
  type t =
    | Published
    | HeldForReview
    | Rejected
    | LikelySpam;
  let decode = (status: string) =>
    switch (status) {
    | "published" => Published
    | "heldForReview" => HeldForReview
    | "rejected" => Rejected
    | "likelySpam" => LikelySpam
    | _ => raise(InvalidStatus)
    };
};

module Sentiment = {
  type t =
    | Hateful
    | Negative
    | Neutral
    | Positive;
  exception InvalidSentiment;
  let decode = (sentiment: string) =>
    switch (sentiment) {
    | "positive" => Positive
    | "neutral" => Neutral
    | "negative" => Negative
    | "hateful" => Hateful
    | _ => raise(InvalidSentiment)
    };
  let encode = (sentiment: t) =>
    switch (sentiment) {
    | Positive => "positive"
    | Neutral => "neutral"
    | Negative => "negative"
    | Hateful => "hateful"
    };
};

module Comment = {
  type t = {
    id: Model.id,
    body: string,
    sentiment: Sentiment.t,
    status: Status.t,
    publishedAt: Moment.t,
  };
  include
    JsonApi.MakeDecoder(
      {
        type nonrec t = t;
        type attributes = {
          body: string,
          sentiment: string,
          status: string,
          publishedAt: Moment.t,
        };
        let attributesDecoder = (json: Js.Json.t) : attributes =>
          Json.Decode.{
            body: json |> field("body", string),
            sentiment: json |> field("sentiment", string),
            status: json |> field("status", string),
            publishedAt: json |> field("published_at", string) |> moment,
          };
        let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : t =>
          switch (resource.attributes) {
          | None => {
              id: resource.id,
              body: "",
              sentiment: Neutral,
              status: Published,
              publishedAt: momentNow(),
            }
          | Some(attributes) => {
              id: resource.id,
              body: attributes.body,
              sentiment: Sentiment.decode(attributes.sentiment),
              status: Status.decode(attributes.status),
              publishedAt: attributes.publishedAt,
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

let fetchAll =
    (
      ~sentiment: option(Sentiment.t)=?,
      callback: Callback.t(list(Comment.t), unit),
    ) => {
  let path =
    switch (sentiment) {
    | None => "/comments"
    | Some(sentiment) =>
      let encodedSentiment = Sentiment.encode(sentiment);
      {j|/comments?sentiment=$(encodedSentiment)|j};
    };
  Api.request(
    ~method=Fetch.Get,
    ~path,
    ~callback=
      response =>
        switch (response) {
        | Success(json) => callback(Success(json |> Comment.decodeMany))
        | Error(_error) => callback(Error())
        },
    (),
  );
};

let rejectAll = (callback: Callback.t(unit, unit), ids: list(Model.id)) =>
  Api.request(
    ~method=Fetch.Post,
    ~path="/rejections",
    ~body=[Json.Encode.("rejection", object_([("ids", list(int, ids))]))],
    ~callback=
      response =>
        switch (response) {
        | Success(_json) => callback(Success())
        | Error(_error) => callback(Error())
        },
    (),
  );

let reject = (callback: Callback.t(unit, unit), comment: Comment.t) =>
  rejectAll(callback, [comment.id]);
