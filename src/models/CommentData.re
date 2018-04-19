open Belt;

open MomentRe;

module Status = {
  exception InvalidStatus;
  type t =
    | Published
    | HeldForReview
    | Rejected
    | LikelySpam;
  let encode = (status: t) =>
    switch (status) {
    | Published => "published"
    | HeldForReview => "heldForReview"
    | Rejected => "rejected"
    | LikelySpam => "likelySpam"
    };
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
  let toString = (sentiment: t) =>
    switch (sentiment) {
    | Positive => "Positive"
    | Neutral => "Neutral"
    | Negative => "Negative"
    | Hateful => "Hateful"
    };
};

module Comment = {
  type t = {
    id: Model.id,
    body: string,
    sentiment: Sentiment.t,
    status: Status.t,
    publishedAt: Moment.t,
    videoId: option(string),
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
          videoId: option(string),
        };
        let attributesDecoder = (json: Js.Json.t) : attributes =>
          Json.Decode.{
            body: json |> field("body", string),
            sentiment: json |> field("sentiment", string),
            status: json |> field("status", string),
            publishedAt: json |> field("published_at", string) |> moment,
            videoId: json |> optional(field("video_id", string)),
          };
        let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : t =>
          switch (resource.attributes) {
          | None => {
              id: resource.id,
              body: "",
              sentiment: Neutral,
              status: Published,
              publishedAt: momentNow(),
              videoId: None,
            }
          | Some(attributes) => {
              id: resource.id,
              body: attributes.body,
              sentiment: Sentiment.decode(attributes.sentiment),
              status: Status.decode(attributes.status),
              publishedAt: attributes.publishedAt,
              videoId: attributes.videoId,
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
  let publishedComments = (comments: list(t)) =>
    comments |> List.keep(_, comment => comment.status == Published);
};

type response =
  Callback.response(JsonApi.Document.decodedMany(Comment.t), unit);

let fetchAll =
    (
      ~page=1,
      ~status: list(Status.t),
      ~sentiment: list(Sentiment.t),
      callback: Callback.t(JsonApi.Document.decodedMany(Comment.t), unit),
    ) => {
  let query = [
    ("page", string_of_int(page)),
    (
      "status",
      status
      |> List.map(_, status => Status.encode(status))
      |> String.concat(","),
    ),
    (
      "sentiment",
      sentiment
      |> List.map(_, sentiment => Sentiment.encode(sentiment))
      |> String.concat(","),
    ),
  ];
  Api.request(
    ~method=Fetch.Get,
    ~path="/comments",
    ~query,
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
